
#define CRYPTO_BEGIN 0
#define CRYPTO_WRITE 1
#define CRYPTO_READ 2
#define CRYPTO_END 3 
struct trace_message {
  unsigned long addr;
  int type;
  char data;
};
