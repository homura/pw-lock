#define RISCV_PGSIZE 4096
#define inline
#include "ckb_syscalls.h"
#include "pw_anyone_can_pay.c"


int LLVMFuzzerTestOneInput(uint8_t *data, size_t size) {
  if (size < sizeof(input_t))
    return 0;
  memcpy(&g_input, data, sizeof(input_t));

  simulator_main();
  return 0;
}
