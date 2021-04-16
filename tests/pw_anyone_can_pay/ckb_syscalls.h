#ifndef CKB_C_STDLIB_CKB_SYSCALLS_H_
#define CKB_C_STDLIB_CKB_SYSCALLS_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "blockchain.h"

#define SYS_exit 93
#define SYS_ckb_load_transaction 2051
#define SYS_ckb_load_script 2052
#define SYS_ckb_load_tx_hash 2061
#define SYS_ckb_load_script_hash 2062
#define SYS_ckb_load_cell 2071
#define SYS_ckb_load_header 2072
#define SYS_ckb_load_input 2073
#define SYS_ckb_load_witness 2074
#define SYS_ckb_load_cell_by_field 2081
#define SYS_ckb_load_header_by_field 2082
#define SYS_ckb_load_input_by_field 2083
#define SYS_ckb_load_cell_data_as_code 2091
#define SYS_ckb_load_cell_data 2092
#define SYS_ckb_debug 2177

#define CKB_SUCCESS 0
#define CKB_INDEX_OUT_OF_BOUND 1
#define CKB_ITEM_MISSING 2
#define CKB_LENGTH_NOT_ENOUGH 3

#define CKB_SOURCE_INPUT 1
#define CKB_SOURCE_OUTPUT 2
#define CKB_SOURCE_CELL_DEP 3
#define CKB_SOURCE_HEADER_DEP 4
#define CKB_SOURCE_GROUP_INPUT 0x0100000000000001
#define CKB_SOURCE_GROUP_OUTPUT 0x0100000000000002

#define CKB_CELL_FIELD_CAPACITY 0
#define CKB_CELL_FIELD_DATA_HASH 1
#define CKB_CELL_FIELD_LOCK 2
#define CKB_CELL_FIELD_LOCK_HASH 3
#define CKB_CELL_FIELD_TYPE 4
#define CKB_CELL_FIELD_TYPE_HASH 5
#define CKB_CELL_FIELD_OCCUPIED_CAPACITY 6

#define CKB_HEADER_FIELD_EPOCH_NUMBER 0
#define CKB_HEADER_FIELD_EPOCH_START_BLOCK_NUMBER 1
#define CKB_HEADER_FIELD_EPOCH_LENGTH 2

#define CKB_INPUT_FIELD_OUT_POINT 0
#define CKB_INPUT_FIELD_SINCE 1

typedef struct input_t {
  uint8_t lock_script_args[20];
  uint8_t witness_lock[72];
  uint8_t tx_hash[32];
} input_t;

static input_t g_input;

mol_seg_t build_bytes(const uint8_t* data, uint32_t len) {
  mol_builder_t b;
  mol_seg_res_t res;
  MolBuilder_Bytes_init(&b);
  for (uint32_t i = 0; i < len; i++) {
    MolBuilder_Bytes_push(&b, data[i]);
  }
  res = MolBuilder_Bytes_build(b);
  return res.seg;
}

mol_seg_t build_script(const uint8_t* code_hash, uint8_t hash_type,
                       const uint8_t* args, uint32_t args_len) {
  mol_builder_t b;
  mol_seg_res_t res;
  MolBuilder_Script_init(&b);

  MolBuilder_Script_set_code_hash(&b, code_hash, 32);
  MolBuilder_Script_set_hash_type(&b, hash_type);
  mol_seg_t bytes = build_bytes(args, args_len);
  MolBuilder_Script_set_args(&b, bytes.ptr, bytes.size);

  res = MolBuilder_Script_build(b);
  free(bytes.ptr);
  return res.seg;
}

int ckb_exit(int8_t code) { return 0;}

int ckb_load_tx_hash(void* addr, uint64_t* len, size_t offset) {
  memcpy(addr, g_input.tx_hash, 32);
  *len = 32;
  return 0;
}

int ckb_checked_load_tx_hash(void* addr, uint64_t* len, size_t offset) {
  return 0;
}

int ckb_load_script_hash(void* addr, uint64_t* len, size_t offset) {
  return 0;
}

int ckb_checked_load_script_hash(void* addr, uint64_t* len, size_t offset) {
  return 0;
}

int ckb_load_cell(void* addr, uint64_t* len, size_t offset, size_t index,
                  size_t source) {
  return 0;
}

int ckb_checked_load_cell(void* addr, uint64_t* len, size_t offset,
                          size_t index, size_t source) {
  return 0;
}

int ckb_load_input(void* addr, uint64_t* len, size_t offset, size_t index,
                   size_t source) {
  return 0;
}

int ckb_checked_load_input(void* addr, uint64_t* len, size_t offset,
                           size_t index, size_t source) {
  return 0;
}

int ckb_load_header(void* addr, uint64_t* len, size_t offset, size_t index,
                    size_t source) {
  return 0;
}

int ckb_checked_load_header(void* addr, uint64_t* len, size_t offset,
                            size_t index, size_t source) {
  return 0;
}

int ckb_load_witness(void* addr, uint64_t* len, size_t offset, size_t index,
                     size_t source) {
  if (index > 1) {
    return 1;  // CKB_INDEX_OUT_OF_BOUND;
  }
  mol_builder_t w;
  MolBuilder_WitnessArgs_init(&w);
  mol_seg_t seg = build_bytes(g_input.witness_lock, 65);
  MolBuilder_WitnessArgs_set_lock(&w, seg.ptr, seg.size);
  free(seg.ptr);

  mol_seg_res_t res = MolBuilder_WitnessArgs_build(w);

  if (res.seg.size <= offset) {
    *len = 0;
    return 0;
  }
  if (addr == NULL) {
    *len = res.seg.size;
    return 0;
  }

  uint32_t remaining = res.seg.size - offset;
  if (remaining > *len) {
    memcpy(addr, res.seg.ptr + offset, *len);
    // keep "len" unchanged
  } else {
    memcpy(addr, res.seg.ptr + offset, remaining);
    *len = remaining;
  }
  free(res.seg.ptr);
  return 0;
}



int ckb_checked_load_witness(void* addr, uint64_t* len, size_t offset,
                             size_t index, size_t source) {
  return 0;
}

int ckb_load_script(void* addr, uint64_t* len, size_t offset) {
  uint8_t code_hash[32] = {0};
  uint8_t hash_type = 0;
  mol_seg_t script = build_script(code_hash, hash_type, g_input.lock_script_args, sizeof(g_input.lock_script_args));
  memcpy(addr, script.ptr, script.size);
  *len = script.size;

  free(script.ptr);

  return 0;
}

int ckb_checked_load_script(void* addr, uint64_t* len, size_t offset) {
  return 0;
}

int ckb_load_transaction(void* addr, uint64_t* len, size_t offset) {
  return 0;
}

int ckb_checked_load_transaction(void* addr, uint64_t* len, size_t offset) {
  return 0;
}

int ckb_load_cell_by_field(void* addr, uint64_t* len, size_t offset,
                           size_t index, size_t source, size_t field) {
  if (source == CKB_SOURCE_CELL_DEP && field == CKB_CELL_FIELD_DATA_HASH) {
    static uint8_t ckb_secp256k1_data_hash[32] = {
        151, 153, 190, 226, 81, 185, 117, 184, 44, 69, 160, 33, 84, 206, 40, 206, 200, 156, 88, 83, 236, 193, 77, 18,
        183, 184, 204, 207, 193, 158, 10, 244
    };
    memcpy(addr, ckb_secp256k1_data_hash, 32);
  }
  return 0;
}

int ckb_checked_load_cell_by_field(void* addr, uint64_t* len, size_t offset,
                                   size_t index, size_t source, size_t field) {
  return 0;
}

int ckb_load_header_by_field(void* addr, uint64_t* len, size_t offset,
                             size_t index, size_t source, size_t field) {
  return 0;
}

int ckb_checked_load_header_by_field(void* addr, uint64_t* len, size_t offset,
                                     size_t index, size_t source,
                                     size_t field) {
  return 0;
}

int ckb_load_input_by_field(void* addr, uint64_t* len, size_t offset,
                            size_t index, size_t source, size_t field) {
  return 0;
}

int ckb_checked_load_input_by_field(void* addr, uint64_t* len, size_t offset,
                                    size_t index, size_t source, size_t field) {
  return 0;
}

int ckb_load_cell_code(void* addr, size_t memory_size, size_t content_offset,
                       size_t content_size, size_t index, size_t source) {
  return 0;
}

int ckb_load_cell_data(void* addr, uint64_t* len, size_t offset, size_t index,
                       size_t source) {
  return 0;
}

int ckb_checked_load_cell_data(void* addr, uint64_t* len, size_t offset,
                               size_t index, size_t source) {
  return 0;
}

int ckb_debug(const char* s) {
  return 0;
}

int ckb_load_actual_type_witness(uint8_t* buf, uint64_t* len, size_t index,
                                 size_t* type_source) {
  return 0;
}

int ckb_calculate_inputs_len() {
  return 0;
}

int ckb_look_for_dep_with_hash(const uint8_t* data_hash, size_t* index) {
  return 0;
}


#endif
