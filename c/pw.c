/* UDT anyone-can-pay lock script
 * refer
 * https://github.com/nervosnetwork/ckb-production-scripts/blob/master/c/anyone_can_pay.c
 *
 * For simplify, we call a cell with anyone-can-pay lock a wallet cell.
 *
 * Wallet cell can be unlocked without a signature, if:
 *
 * 1. There is 1 output wallet cell that has the same type hash with the
 * unlocked wallet cell.
 * 2. The UDT or CKB(if type script is none) in the output wallet is more than
 * the unlocked wallet.
 * 3. if the type script is none, the cell data is empty.
 *
 * otherwise, the script perform secp256k1_keccak256_sighash_all verification.
 */

// uncomment to enable printf in CKB-VM
//#define CKB_C_STDLIB_PRINTF
//#include <stdio.h>

#include "ckb_syscalls.h"
#include "defs.h"
#include "overflow_add.h"
#include "protocol.h"
#include "pw_lock.h"
#include "quick_pow10.h"
#include "secp256k1_helper.h"

#define BLAKE2B_BLOCK_SIZE 32
#define SCRIPT_SIZE 32768
#define CKB_LEN 8
#define UDT_LEN 16
#define MAX_WITNESS_SIZE 32768
#define MAX_TYPE_HASH 256

typedef struct {
  int is_ckb_only;
  unsigned char type_hash[BLAKE2B_BLOCK_SIZE];
  uint64_t ckb_amount;
  uint128_t udt_amount;
  uint32_t output_cnt;
} InputWallet;

int has_signature(int *has_sig) {
  int ret = 0;

  // extra lock from witness without molecule tools, to support very large
  // input_type and output_type
  uint8_t witness[SIGNATURE_WITNESS_BUFFER_SIZE];
  uint64_t first_witness_len = SIGNATURE_WITNESS_BUFFER_SIZE;
  ret = ckb_load_witness(witness, &first_witness_len, 0, 0,
                         CKB_SOURCE_GROUP_INPUT);
  if (ret != CKB_SUCCESS) {
    *has_sig = 0;
    return CKB_SUCCESS;
  }

  size_t readed_len = first_witness_len;
  if (readed_len > SIGNATURE_WITNESS_BUFFER_SIZE) {
    readed_len = SIGNATURE_WITNESS_BUFFER_SIZE;
  }
  if (readed_len < 20) {
    *has_sig = 0;
    return CKB_SUCCESS;
  }
  uint32_t lock_length = *((uint32_t *)(&witness[16]));
  if (readed_len < 20 + lock_length) {
    *has_sig = 0;
  } else {
    *has_sig = lock_length > 0;
  }
  return CKB_SUCCESS;
}

#ifdef CKB_SIMULATOR
int simulator_main() {
#else
int main() {
#endif
  uint8_t code_buffer[MAX_CODE_SIZE] __attribute__((aligned(RISCV_PGSIZE)));
  uint64_t code_buffer_size = MAX_CODE_SIZE;

  int ret;
  int has_sig;
  ret = has_signature(&has_sig);
  if (ret != CKB_SUCCESS) {
    return ret;
  }
  if (!has_sig) return -1;
  /* unlock via signature */
  return verify_pwlock_sighash_all(code_buffer, code_buffer_size);
}
