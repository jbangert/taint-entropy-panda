enum trace_type{CRYPTO_BEGIN=0, CRYPTO_WRITE=1, CRYPTO_READ=2, CRYPTO_END=3};
struct trace_message {
  unsigned long addr;
  trace_type type;
  char data;
};
