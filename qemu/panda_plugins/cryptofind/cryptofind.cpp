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
}

#include "iclass.h"

//#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <set>
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
void *plugin_self;
int after_block_translate(CPUState *env, TranslationBlock *tb){
  target_phys_addr_t physaddr = panda_virt_to_phys(env,tb->pc);
  unsigned char *buf = (unsigned char *) malloc(tb->size);
  int err = panda_virtual_memory_rw(env, tb->pc, buf, tb->size, 0);
  if(err == -1){
    fprintf(stderr, "Error reading block %lX\n", tb->pc);
  }
  bool use64 = (env->hflags & HF_LMA_MASK) != 0;
  blockinfo blk = get_block_stats(buf, tb->pc, tb->size, use64);
  if(blk.total_instr >= 10 && blk.arith_instr >= 0.4 * blk.total_instr)
    printf("Block stats %lX %d %d\n",physaddr, blk.total_instr, blk.arith_instr);
  return 0;
}
extern void init_icls();
bool init_plugin(void *self) {
  
    printf ("Initializing cryptofind\n");
    plugin_self = self;
    init_icls();


    panda_cb pcb;
    pcb.after_block_translate  = after_block_translate;
    panda_register_callback(self, PANDA_CB_AFTER_BLOCK_TRANSLATE, pcb);
    // register the tstringsearch_match fn to be called at the on_ssm site within panda_stringsearch
    //PPP_REG_CB("", on_ssm, tstringsearch_match) ;
    // PPP_REG_CB("callstack_instr", on_call, tentropy_oncall);
    //PPP_REG_CB("callstack_instr", on_ret, tentropy_onret);
    return true;

}


void uninit_plugin(void *self) {
}
