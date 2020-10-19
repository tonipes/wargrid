// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <kb/freelist.h>
#include <kb/table.h>
#include <kb/crt.h>
#include <kb/log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KB_RESOURCE_HASHED_FUNC_DECLS(t_name, handle_t, create_info_t)                              \
  void      kb_##t_name##_set_name(handle_t handle, const char* name);                              \
  void      kb_##t_name##_set_hash(handle_t handle, kb_hash hash);                                  \
  void      kb_##t_name##_remove_name(handle_t handle);                                             \
  bool      kb_##t_name##_has(const char* name);                                                    \
  handle_t  kb_##t_name##_get(const char* name);                                                    \
  bool      kb_##t_name##_has_hash(kb_hash hash);                                                   \
  handle_t  kb_##t_name##_get_hash(kb_hash hash);                                                   \
  handle_t  kb_##t_name##_get_or_alloc(kb_hash hash);                                               \
  void      kb_##t_name##_construct(handle_t h, const create_info_t* info);                         \
  void      kb_##t_name##_destruct(handle_t h);                                                     

#define KB_RESOURCE_ALLOC_FUNC_DECLS(t_name, handle_t, create_info_t)                               \
  handle_t kb_##t_name##_allocate();                                                                \
  handle_t kb_##t_name##_create(const create_info_t info);                                          \
  void     kb_##t_name##_destroy(handle_t handle);                                                  \
  uint32_t kb_##t_name##_count();                                                                   \
  void     kb_##t_name##_purge();                                                                   \

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

template <typename kb_handle, typename create_info_t, uint16_t cap>
struct kb_resource_slot_allocator {
  kb_resource_slot_allocator() {
    kb_table_create     (&table, cap);
    kb_freelist_create  (&freelist, cap);
    kb_memset(&refcount, 0, sizeof(refcount));
  }

  ~kb_resource_slot_allocator() {
    kb_table_destroy     (&table);
    kb_freelist_destroy  (&freelist);
  }
  
  void purge() {
    uint32_t* dense = kb_freelist_get_dense(&freelist);
    for (uint32_t i = 0; i < freelist.count; ++i) {
      kb_log_debug("Purge: ({}) {}", i, dense[i]);
    }
  }

  uint16_t      refcount  [cap];
  kb_freelist   freelist;
  kb_table      table;
};

#define KB_RESOURCE_STORAGE_DEF(t_name, handle_t, ref_t, cap)                                     \
  ref_t t_name##_refs [cap];                                                                      \
  inline ref_t& t_name##_ref(handle_t handle) {                                                   \
    return t_name##_refs[kb_to_arr(handle)];                                                      \
  }           

#define KB_RESOURCE_ALLOC_FUNC_DEF(t_name, handle_t, create_info_t, cap)                          \
  kb_resource_slot_allocator<handle_t, create_info_t, cap> t_name##_data;                         \
  void kb_##t_name##_remove_name(handle_t handle) {                                               \
    kb_table_remove(&(t_name##_data.table), kb_to_arr(handle));                                   \
  }                                                                                               \
  handle_t  kb_##t_name##_allocate() {                                                            \
    return handle_t##_from_arr(kb_freelist_take(&(t_name##_data.freelist)));                      \
  }                                                                                               \
  void kb_##t_name##_free(handle_t handle) {                                                      \
    kb_freelist_return(&(t_name##_data.freelist), kb_to_arr(handle));                             \
  }                                                                                               \
  void kb_##t_name##_destroy(handle_t handle) {                                                   \
    kb_##t_name##_free(handle);                                                                   \
    kb_##t_name##_remove_name(handle);                                                            \
    kb_##t_name##_destruct(handle);                                                               \
  }                                                                                               \
  handle_t kb_##t_name##_create(const create_info_t info) {                                       \
    handle_t handle = kb_##t_name##_allocate();                                                   \
    kb_##t_name##_construct(handle, &info);                                                       \
    return handle;                                                                                \
  }                                                                                               \
  uint32_t kb_##t_name##_count() {                                                                \
    return kb_freelist_count(&(t_name##_data.freelist));                                          \
  }                                                                                               \
  void kb_##t_name##_purge() {                                                                    \
    uint32_t* dense = kb_freelist_get_dense(&t_name##_data.freelist);                             \
    uint32_t count = kb_##t_name##_count();                                                       \
    for (uint32_t i = 0; i < count; ++i) {                                                        \
      kb_##t_name##_destroy(handle_t##_from_arr(dense[0]));                                       \
    }                                                                                             \
  }   

#define KB_RESOURCE_DATA_HASHED_DEF(t_name, handle_t)                                             \
  void kb_##t_name##_set_hash(handle_t handle, kb_hash _hash) {                                   \
    kb_table_insert(&(t_name##_data.table), _hash, kb_to_arr(handle));                            \
  }                                                                                               \
  bool kb_##t_name##_has_hash(kb_hash _hash) {                                                    \
    return kb_is_valid(handle_t##_from_arr(kb_table_get(&(t_name##_data.table), _hash)));         \
  }                                                                                               \
  handle_t kb_##t_name##_get_hash(kb_hash _hash) {                                                \
    return handle_t##_from_arr(kb_table_get(&(t_name##_data.table), _hash));                      \
  }                                                                                               \
  void kb_##t_name##_set_name(handle_t handle, const char* name) {                                \
    kb_##t_name##_set_hash(handle, kb_hash_string(name));                                         \
  }                                                                                               \
  bool kb_##t_name##_has(const char* name) {                                                      \
    return kb_##t_name##_has_hash(kb_hash_string(name));                                          \
  }                                                                                               \
  handle_t kb_##t_name##_get(const char* name) {                                                  \
    return kb_##t_name##_get_hash(kb_hash_string(name));                                          \
  }                                                                                               \
  handle_t kb_##t_name##_get_or_alloc(kb_hash hash) {                                             \
    if (kb_##t_name##_has_hash(hash)) return kb_##t_name##_get_hash(hash);                        \
    handle_t handle = kb_##t_name##_allocate();                                                   \
    kb_##t_name##_set_hash(handle, hash);                                                         \
    return handle;                                                                                \
  }                                                                                               \

#endif
