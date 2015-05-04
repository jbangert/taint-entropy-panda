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
}

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
std::unordered_map<target_ulong, uint8_t> read_set;
std::unordered_map<target_ulong, uint8_t> write_set;
blockinfo cumulative;
bool tracing;
void *plugin_self;
FILE *f_memtrace;
#include "memtrace.h"
#ifdef TARGET_I386
static inline bool heuristic(blockinfo &blk){
  return blk.total_instr >= 16 && blk.arith_instr >= blk.total_instr/2;
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
  }
  return 0;
}
static void memtrace(int t, target_ulong addr, char buf){
  if(!tracing)
    return;
  trace_message m ={};
  m.type= t;
  m.addr = addr;
  m.data = buf;
  fwrite(&m,sizeof m,1,f_memtrace);
}
static void dump_memsets(){
  for(auto &c : read_set){
    memtrace(CRYPTO_READ,c.first,c.second);
  }
  for(auto &c : write_set){
    memtrace(CRYPTO_WRITE,c.first,c.second);
  }  
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
int before_block_exec(CPUState *env, TranslationBlock *tb){
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
      cumulative = blk;
      tracing = true;
      panda_enable_memcb();
      read_set.clear();
      write_set.clear();
      #ifdef CONFIG_SOFTMMU
      memtrace(CRYPTO_BEGIN,rr_get_guest_instr_count(),0);
      #else
      memtrace(CRYPTO_BEGIN,tb->pc,0);
      #endif
      
    }
  } else {
    if(!heuristic(cumulative)){
      //The last block we executed was the last crypto block
      dump_memsets();
      #ifdef CONFIG_SOFTMMU
      memtrace(CRYPTO_END,rr_get_guest_instr_count(),0);
      #else
      memtrace(CRYPTO_END,tb->pc,0);
      #endif
      tracing = false;
      panda_disable_memcb();
    } else {
      cumulative.total_instr  +=  blk.total_instr;
      cumulative.arith_instr  +=  blk.arith_instr;
    }
  }
  return 0;
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
    pcb.before_block_exec  = before_block_exec;
    panda_register_callback(self, PANDA_CB_BEFORE_BLOCK_EXEC, pcb);
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
    f_memtrace = fopen("memtrace","wb");
    cumulative = {0,0};
    tracing = false;
    return true;

}


void uninit_plugin(void *self) {
  fflush(f_memtrace);
  fclose(f_memtrace);
}
