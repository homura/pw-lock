#ifndef PW_LOCK_TESTS_PW_ANYONE_CAN_PAY_CKB_SWAPPABLE_SIGNATURES_H_
#define PW_LOCK_TESTS_PW_ANYONE_CAN_PAY_CKB_SWAPPABLE_SIGNATURES_H_


typedef int (*verify_function)(void *, const uint8_t *, size_t, const uint8_t *,
                               size_t, uint8_t *, size_t *);
typedef struct {
  uint8_t *code_buffer;
  uint64_t code_buffer_size;
  uint8_t *prefilled_data_buffer;
  uint64_t prefilled_buffer_size;
  verify_function verify_func;
} CkbSwappableSignatureInstance;

int ckb_initialize_swappable_signature(
    const uint8_t *code_hash, uint8_t hash_type,
    CkbSwappableSignatureInstance *instance) {
  return -1;
}

#endif //PW_LOCK_TESTS_PW_ANYONE_CAN_PAY_CKB_SWAPPABLE_SIGNATURES_H_
