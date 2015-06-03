/* PANDABEGINCOMMENT
 * 
 * Authors:
 *  Tim Leek               tleek@ll.mit.edu
 *  Ryan Whelan            rwhelan@ll.mit.edu
 *  Joshua Hodosh          josh.hodosh@ll.mit.edu
 *  Michael Zhivich        mzhivich@ll.mit.edu
 *  Brendan Dolan-Gavitt   brendandg@gatech.edu
 * 
 * This work is licensed under the terms of the GNU GPL, version 2. 
 * See the COPYING file in the top-level directory. 
 * 
 PANDAENDCOMMENT */
// This needs to be defined before anything is included in order to get
// the PRIx64 macro
#define __STDC_FORMAT_MACROS

#include "../../panda/panda_addr.h"
#include "../taint2/taint2.h"
extern "C" {

#include <dlfcn.h>
#include "config.h"
#include "qemu-common.h"
#include "monitor.h"
#include "cpu.h"
#include "panda_plugin.h"
#include "rr_log.h"

#include "../callstack_instr/callstack_instr.h"
  //#include "../callstack_instr/callstack_instr_ext.h"
  

#include "panda_plugin_plugin.h"
#include "panda_common.h"

#include "../taint2/taint2_ext.h"
}

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

#ifdef CONFIG_SOFTMMU
enum taint_source{
  taint_FreeBSD
};
taint_source taint_source = taint_FreeBSD;
void *plugin_self;
char function[8192];
size_t function_size=0;
std::set<target_phys_addr_t> interesting_funcs;
//Every time we jump to a function and it has changed, it will be retranslated, so it suffices to check
// on every retranslation
int tentropy_enable_taint(){
  if(!taint2_enabled()){
    uint64_t ic = rr_get_guest_instr_count();
    printf ("enabling taint2 at instr count %" PRIu64 "\n", ic);
    taint2_enable_taint();
  }
  return 0;
}
int after_block_translate(CPUState *env, TranslationBlock *tb){
  //Just scan the entire function
  target_phys_addr_t physaddr = panda_virt_to_phys(env,tb->pc);
  uint8_t *buf = (uint8_t *)malloc(function_size);
  assert(buf);
  panda_virtual_memory_rw(env,tb->pc,buf,function_size,0);

  if(!memcmp(function,buf,function_size))
    interesting_funcs.insert(physaddr);
  else
    interesting_funcs.erase(physaddr);
  free(buf);
  return 0;
}
void tentropy_oncall(CPUState *env, target_ulong func){
  target_phys_addr_t physaddr = panda_virt_to_phys(env,func);
  static uint64_t label = 10;
  if(interesting_funcs.count(physaddr)>0){
    printf("Function %llX with taint called \n", (long long unsigned)func);
    if(taint_source == taint_FreeBSD){
      tentropy_enable_taint();
      #ifdef TARGET_X86_64
      uint64_t va_begin = env->regs[R_EDI];
      uint64_t pa_begin = panda_virt_to_phys(env,va_begin);
      uint64_t size = env->regs[R_ESI];
      assert(size<32);
      for(;size;size--){
        printf("Tainting %" PRIu64 "\n",pa_begin+size);
        taint2_label_ram(pa_begin+size , label++); //TODO: generate unique label
      }
      #endif
    }
  }
}
void tentropy_onret(CPUState *env, target_ulong func){
  //  printf("Function %llX returned\n", (long long unsigned)func);
}

bool init_plugin(void *self) {
  
    printf ("Initializing tentropy\n");

    plugin_self = self;

    panda_require("callstack_instr");
    panda_require("taint2");
    

    panda_arg_list *args;
    args = panda_get_args("tentropy");
    
    const char *filename  = panda_parse_string(args, "random_harvest_func", "random_harvest.func");
    FILE *datafile = fopen(filename,"r");
    assert(datafile);
    function_size = fread(function,1,sizeof function,datafile);
    fclose(datafile);
#ifndef CONFIG_SOFTMMU
    fprintf(stderr,"No CONFIG_SOFTMMU\n");
    return false;
#else

    // this sets up the taint api fn ptrs so we have access
    assert(init_taint2_api());
    panda_cb pcb;
    pcb.after_block_translate  = after_block_translate;
    panda_register_callback(self, PANDA_CB_AFTER_BLOCK_TRANSLATE, pcb);
    // register the tstringsearch_match fn to be called at the on_ssm site within panda_stringsearch
    //PPP_REG_CB("", on_ssm, tstringsearch_match) ;
    PPP_REG_CB("callstack_instr", on_call, tentropy_oncall);
    PPP_REG_CB("callstack_instr", on_ret, tentropy_onret);
    return true;
#endif
}


void uninit_plugin(void *self) {
}
  
#endif
