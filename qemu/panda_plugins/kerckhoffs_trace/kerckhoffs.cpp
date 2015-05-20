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
FILE *datafile;
bool init_plugin(void *self) {
  
    printf ("Initializing kerckhoffs\n");

    plugin_self = self;

    

    panda_arg_list *args;
    args = panda_get_args("kerckhoffs");
    
    const char *filename  = panda_parse_string(args, "logfile", "kerckhoffs.log");
    FILE *datafile = fopen(filename,"w");
    assert(datafile);

    // this sets up the taint api fn ptrs so we have access
    assert(init_taint2_api());
    panda_cb pcb;
    pcb.after_block_translate  = after_block_translate;
    panda_register_callback(self, PANDA_CB_AFTER_BLOCK_TRANSLATE, pcb);
    // register the tstringsearch_match fn to be called at the on_ssm site within panda_stringsearch
    //PPP_REG_CB("", on_ssm, tstringsearch_match) ;
    // PPP_REG_CB("callstack_instr", on_call, tentropy_oncall);
    //PPP_REG_CB("callstack_instr", on_ret, tentropy_onret);
    return true;
#endif
}


void uninit_plugin(void *self) {
}
