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

extern "C" {
#include "panda_plugin.h"
#include "panda_common.h"
#include "pandalog.h"
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
std::unordered_map<target_ulong , blockinfo> block_infos;
blockinfo cumulative;
bool tracing;
void *plugin_self;
FILE *memtrace;
#include "memtrace.h"
#ifdef TARGET_I386
static inline bool heuristic(blockinfo &blk){
  return blk.total_instr >= 16 && blk.arith_instr >= blk.total_instr/2;
}
int after_block_translate(CPUState *env, TranslationBlock *tb){
  
  target_phys_addr_t physaddr = panda_virt_to_phys(env,tb->pc);
  unsigned char *buf = (unsigned char *) malloc(tb->size);
  int err = panda_virtual_memory_rw(env, tb->pc, buf, tb->size, 0);
  if(err == -1){
    fprintf(stderr, "Error reading block %lX\n", (unsigned long)tb->pc);
  }
  bool use64 = (env->hflags & HF_LMA_MASK) != 0;
  blockinfo blk = get_block_stats(buf, tb->pc, tb->size, use64);
  block_infos[physaddr] = blk;
  if(heuristic(blk)){
    printf("Block stats %lX %d %d\n",physaddr, blk.total_instr, blk.arith_instr);
  }
  return 0;
}
static void print_memaccess(trace_type t,target_ulong pc, target_ulong addr, target_ulong size, void *buf){
  if(!tracing)
    return;
  trace_message m;
  m.type= t;
  m.pc = pc;
  m.addr = addr;
  m.size = size;
  if(size>8){
    printf("Crypto\t%u\t" "%lx\t" "%lX\t" "%lX\t",t,(unsigned long)pc,(unsigned long)addr, (unsigned long)size);
    fprintf(stderr,"Write larger than 8, size=%lX\n", (unsigned long) size);
  } else {
    memcpy(m.data,buf,size);
  }
  fwrite(&m,sizeof m,1,memtrace);
}
int vmem_write(CPUState *env, target_ulong pc, target_ulong addr, target_ulong size, void *buf){
  print_memaccess(CRYPTO_WRITE, pc,addr,size,buf);
  return 0;
}
int vmem_read(CPUState *env, target_ulong pc, target_ulong addr, target_ulong size, void *buf){
  print_memaccess(CRYPTO_READ, pc,addr,size,buf);
  return 0;
}
int before_block_exec(CPUState *env, TranslationBlock *tb){
  target_phys_addr_t physaddr = panda_virt_to_phys(env,tb->pc);
  blockinfo &blk = block_infos[physaddr];
  if(!tracing){
    if(heuristic(blk)){
      cumulative = blk;
      tracing = true;
      panda_enable_memcb();
      print_memaccess(CRYPTO_BEGIN,0,0,0,NULL);
    }
  } else {
    if(!heuristic(cumulative)){
      //The last block we executed was the last crypto block
      tracing = false;
      panda_disable_memcb(); 
      print_memaccess(CRYPTO_END,0,0,0,NULL);
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
    memtrace = fopen("memtrace","wb");
    cumulative = {0,0};
    tracing = false;
    return true;

}


void uninit_plugin(void *self) {
}
