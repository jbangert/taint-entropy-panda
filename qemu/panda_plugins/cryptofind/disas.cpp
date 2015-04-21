#include <distorm.h>
#include <mnemonics.h>
#include "iclass.h"
extern "C" {
  #include "panda_plugin.h"
}
#include <stdio.h>
blockinfo get_block_stats(const uint8_t *buf, unsigned long pc, size_t size, bool use64bit){

  blockinfo retval = {0,0};
   
#if defined(TARGET_I386)
   
    _DInst dec[256]; 
    unsigned int dec_count = 0;
    _DecodeType dt = use64bit ? Decode64Bits : Decode32Bits;

    _CodeInfo ci;
    ci.code = buf;
    ci.codeLen = size;
    ci.codeOffset = pc;
    ci.dt = dt;
    ci.features = DF_NONE;
    distorm_decompose(&ci, dec, 256, &dec_count);
    
    for (int i = dec_count - 1; i >= 0; i--) {
      _DecodedInst inst; 
        if (dec[i].flags == FLAG_NOT_DECODABLE) {
          printf("Instruction not decodable %lX\n", dec[i].addr);
          break;
        }
        switch(icls[dec[i].opcode]){
        case ICLS_NORMAL:
          retval.total_instr++;
          break;
        case ICLS_WARN:
          distorm_format(&ci, &dec[i], &inst);
          
          fprintf(stderr,"Could not classify instruction %s %s\n", inst.mnemonic.p, inst.operands.p);
          retval.total_instr++;
          break;
        case ICLS_BITARITH:
          retval.arith_instr++;
          retval.total_instr++;
          break;
        case ICLS_MOV:
          break;
        }
    }
#else
    fprintf(stderr, "Architecture not supported\n");
#endif
    return retval;

}
