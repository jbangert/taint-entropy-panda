#ifndef ICLASS_H
#define ICLASS_H
#include <stddef.h>
#define ICLS_NORMAL 0
#define ICLS_MOV 1
#define ICLS_BITARITH 2
#define ICLS_WARN 3

struct blockinfo{
  int total_instr, arith_instr;
};
extern char icls[65536];
extern blockinfo get_block_stats(const uint8_t *buf, unsigned long pc, size_t size, bool use64bit);
#endif
