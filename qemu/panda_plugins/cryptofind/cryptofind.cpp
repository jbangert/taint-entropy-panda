/* PANDABEGINCOMMENT
 * 
 * Authors:
 *  Julian Bangert <firstname at csail mit edu> 
 * 
 * This work is licensed under the terms of the GNU GPL, version 2. 
 * See the COPYING file in the top-level directory. 
 * 
 PANDAENDCOMMENT */
// This needs to be defined before anything is included in order to get
// the PRIx64 macro
#define __STDC_FORMAT_MACROS
#ifdef CONFIG_SOFTMMU
#define RAM_SIZE 512*1024*1024
#define FIXED_SIZE_RAM
#endif 

extern "C" {
#include "panda_plugin.h"
#include "panda_common.h"
#include "pandalog.h"
#include  "rr_log.h"
#include "panda_plugin_plugin.h"
#include "../callstack_instr/callstack_instr.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
  
#include <openssl/md5.h>
}
#include "trace.pb.h"
#include "iclass.h"

//#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <unordered_set>
#include <unordered_map>
#include <vector>

//#include <map>
//#include <fstream>
//#include <sstream>
//#include <string>

// These need to be extern "C" so that the ABI is compatible with
// QEMU/PANDA, which is written in C
extern "C" {

bool init_plugin(void *);
void uninit_plugin(void *);

}
//XXX: Do this on a per-address space basis
//std::unordered_map<target_ulong , blockinfo> block_infos;
#ifdef FIXED_SIZE_RAM
std::vector<blockinfo> block_infos;
#else
std::unordered_map<target_ulong, blockinfo> block_infos;
#endif
uint64_t start,end;
std::vector<uint64_t> exec_trace;
std::unordered_map<target_ulong, uint8_t> read_set;
std::unordered_map<target_ulong, uint8_t> write_set;
blockinfo cumulative;
bool tracing;
void *plugin_self;
int f_memtrace;
#ifdef TARGET_I386
void trace_message(::google::protobuf::Message &tr){
  auto ser =   tr.SerializeAsString();
  uint64_t siz  = ser.size();
  assert(sizeof siz == write(f_memtrace, &siz, sizeof siz));
  assert(siz == write(f_memtrace, ser.data(), siz));
}
static inline bool heuristic(blockinfo &blk){
  return blk.total_instr >= 16 && blk.arith_instr >= blk.total_instr/2;
}
std::unordered_map <target_ulong, uint64_t> block_ids;
std::unordered_map<std::string, uint64_t> logged_blocks;
void emplace_block(target_ulong addr,unsigned char *buf, size_t siz){
  unsigned char hashbuf[16];
  MD5(buf,siz,hashbuf);
  std::string hash((const char *)hashbuf, sizeof hashbuf);
  if(logged_blocks.count(hash)==0){
    uint64_t id = logged_blocks.size()+1;
    logged_blocks[hash] = id;
    tentropy::CodeBlock * cb =new tentropy::CodeBlock();
    cb->set_identifier(id);
    cb->set_code(buf,siz);
    tentropy::Trace tr;
    tr.set_allocated_code(cb);
    trace_message(tr);
  }
  block_ids[addr] = logged_blocks[hash];
}
int after_block_translate(CPUState *env, TranslationBlock *tb){
  #ifdef CONFIG_SOFTMMU
  target_phys_addr_t physaddr = panda_virt_to_phys(env,tb->pc);
  #else
  target_ulong physaddr = tb->pc;
  #endif
  #ifdef FIXED_SIZE_RAM
  if(physaddr >= RAM_SIZE){
    fprintf(stderr,"physaddr outside of ram %lX\n", physaddr);
    return 0;
  }
  #endif
  unsigned char *buf = (unsigned char *) malloc(tb->size);
  int err = panda_virtual_memory_rw(env, tb->pc, buf, tb->size, 0);
  if(err == -1){
    fprintf(stderr, "Error reading block %lX\n ", (unsigned long)tb->pc);
    return 0;
  }
  bool use64 = (env->hflags & HF_LMA_MASK) != 0;
  blockinfo blk = get_block_stats(buf, tb->pc, tb->size, use64);
  block_infos[physaddr] = blk;
  if(heuristic(blk)){
    printf("Block stats %lX %d %d\n",physaddr, blk.total_instr, blk.arith_instr);
    emplace_block(tb->pc,buf, tb->size);
  }
  return 0;
}
void vectorize_set(std::map<uint64_t, uint8_t> &m, std::map<uint64_t, std::vector<uint8_t> > &out){
  uint64_t lastaddr = -1;
  int size=-1;
  std::vector<uint8_t> * v;
  for (auto &c: m){
    if(lastaddr +1!= c.first || size  == -1){      
      size = 0;
      v = &out[c.first];
    }
    v->push_back(c.second);
    lastaddr = c.first;
    size++;
  }
}
template <typename functor, typename collection> void trace_memset(const collection &set, const functor &f){
  std::map<uint64_t, uint8_t> ordered;
  for(auto &celem : set){
    ordered.insert(celem);
    }
  uint64_t addr=0;
  /*
  std::map<uint64_t, std::vector<uint8_t> > out;
  vectorize_set(ordered,out);
  for(auto &access: out){
    tentropy::CryptoTrace_MemAccess *memaccess = f();
    memaccess->set_addr(access.first);
    memaccess->set_data(access.second.data(), access.second.size());
    }*/
  tentropy::BlockExecution_MemAccess *memaccess = NULL;
  for(auto &access : ordered){
   
    assert(access.first > addr);
    addr = access.first;
    if(!memaccess || memaccess->addr() + memaccess->data().size() != access.first){
      memaccess = f();
      memaccess->set_addr(access.first);
    }
    memaccess->mutable_data()->push_back(access.second);

    }
}
void trace_cryptoblock(){
  tentropy::BlockExecution * trace =new tentropy::BlockExecution();
  trace->set_start(start);
  trace->set_end(end);
  trace_memset(read_set,  [&]() {return trace->add_read();});
  trace_memset(write_set, [&]() {return trace->add_write();});
  trace->mutable_exec()->Reserve(exec_trace.size());
  for(auto &e: exec_trace){
    trace->mutable_exec()->Add(e);
  }
  tentropy::Trace tr;
  tr.set_allocated_exectrace(trace);
  trace_message(tr);
}
int vmem_read(CPUState *env, target_ulong pc, target_ulong addr, target_ulong size, void *buf){
  if(!tracing) return 0;
  for(target_ulong i=0;i<size;i++)
    read_set.emplace(addr+i, *((uint8_t *)buf + i));
  return 0;
}
int vmem_write(CPUState *env, target_ulong pc, target_ulong addr, target_ulong size, void *buf){
  if(!tracing) return 0;
  for(target_ulong i=0;i<size;i++)
  write_set[addr+i] = *((uint8_t *)buf + i); 
  return 0;
}
bool before_block_exec_invalidate(CPUState *env, TranslationBlock *tb){
  bool invalidate =false;
  #ifdef CONFIG_SOFTMMU
  target_phys_addr_t physaddr = panda_virt_to_phys(env,tb->pc);
  #else
  target_ulong physaddr = tb->pc;
  #endif
  #ifdef FIXED_SIZE_RAM
  if(physaddr >= RAM_SIZE){
    return 0;
  }
  #endif
  blockinfo &blk = block_infos[physaddr];
  if(!tracing){
    if(heuristic(blk)){
      read_set.clear();
      write_set.clear();
      exec_trace.clear();
#ifdef CONFIG_SOFTMMU
      panda_enable_memcb();
      panda_do_flush_tb();
      invalidate = true;
#endif
      #ifdef CONFIG_SOFTMMU
      start = rr_get_guest_instr_count();
      #else
      start = tb->pc;
      #endif
      exec_trace.push_back(block_ids[tb->pc]);
      cumulative = blk;
      tracing = true;
    }
  } else {
    if(!heuristic(cumulative)){
      //The last block we executed was the last crypto block

      #ifdef CONFIG_SOFTMMU
      end = rr_get_guest_instr_count();
      panda_disable_memcb();
      #else
      end = tb->pc;
      trace_cryptoblock();
      #endif
      //fsync(f_memtrace);
      tracing = false;
    } else {
      cumulative.total_instr  +=  blk.total_instr;
      cumulative.arith_instr  +=  blk.arith_instr;
      exec_trace.push_back(block_ids[tb->pc]);
      //log execution of this code
    }
  }
  return invalidate;
}

 #endif
extern void init_icls();
bool init_plugin(void *self) {
  #ifdef TARGET_I386
  #ifdef FIXED_SIZE_RAM
  block_infos.resize(RAM_SIZE);
    fprintf(stderr,"Initializing ram_size, %lX with %u per element\n",RAM_SIZE,sizeof(struct blockinfo));
  #endif

    printf ("Initializing cryptofind\n");
    plugin_self = self;
    init_icls();
    panda_cb pcb;
    pcb.after_block_translate  = after_block_translate;
    panda_register_callback(self, PANDA_CB_AFTER_BLOCK_TRANSLATE, pcb);
    pcb.before_block_exec_invalidate_opt  = before_block_exec_invalidate;
    panda_register_callback(self, PANDA_CB_BEFORE_BLOCK_EXEC_INVALIDATE_OPT, pcb);
    pcb.virt_mem_write = vmem_write;
    panda_register_callback(self, PANDA_CB_VIRT_MEM_WRITE, pcb);
    pcb.virt_mem_read = vmem_read;
    panda_register_callback(self, PANDA_CB_VIRT_MEM_READ, pcb);
    //pcb.before_block_exec = before_block_exec;
    //panda_register_callback(self, PANDA_CB_BEFORE_BLOCK_EXEC, pcb);
    // register the tstringsearch_match fn to be called at the on_ssm site within panda_stringsearch
    //PPP_REG_CB("", on_ssm, tstringsearch_match) ;
    // PPP_REG_CB("callstack_instr", on_call, tentropy_oncall);
    //PPP_REG_CB("callstack_instr", on_ret, tentropy_onret);
    #endif
    //TODO: see if this kills performance too much
    panda_enable_memcb();
    panda_disable_tb_chaining();

    f_memtrace = creat("memtracev2",0666);
    cumulative = {0,0};
    tracing = false;
    return true;

}


void uninit_plugin(void *self) {
  fsync(f_memtrace);
  close(f_memtrace);
}
