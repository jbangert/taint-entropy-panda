#ifndef __CALLSTACK_INSTR_H
#define __CALLSTACK_INSTR_H
typedef struct {
  target_ulong asid;
  target_ulong sp;
} callstack_id;
typedef void (* on_call_t)(CPUState *env, target_ulong func);
typedef void (* on_ret_t)(CPUState *env, target_ulong func);
typedef void (* on_call_ext_t)(CPUState *env, target_ulong func, callstack_id stack);
typedef void (* on_ret_ext_t)(CPUState *env, target_ulong func, callstack_id stack);
#endif
