#define RISCV_PGSIZE 4096
#define inline
#include "ckb_syscalls.h"
#include "pw_anyone_can_pay.c"



int main(int argc, char **argv) {
  fprintf(stderr, "StandaloneFuzzTargetMain: running %d inputs\n", argc - 1);
  for (int i = 1; i < argc; i++) {
    fprintf(stderr, "Running: %s\n", argv[i]);
    FILE *f = fopen(argv[i], "r");
    fseek(f, 0, SEEK_END);
    size_t len = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *buf = (unsigned char *)malloc(len);
    size_t n_read = fread(buf, 1, len, f);
    fclose(f);

    if (n_read >= sizeof(input_t)) {
      memcpy(&g_input, buf, sizeof(input_t));
      simulator_main();
    }
    free(buf);
    fprintf(stderr, "Done:    %s: (%zd bytes)\n", argv[i], n_read);
  }
}
