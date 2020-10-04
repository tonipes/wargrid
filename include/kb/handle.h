#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t kb_handle_idx;

static inline bool kb_is_valid_idx  (kb_handle_idx idx) { return idx != UINT32_MAX && idx != 0; }  \

#ifdef __cplusplus
}
#endif

// TODO: Not sure how to make functions in #ifdef __cplusplus...
#define KB_HANDLE(name_t)                                                                                           \
	extern "C" {                                                                                                      \
    typedef struct { kb_handle_idx idx; } name_t;                                                                   \
  }                                                                                                                 \
  static inline bool          kb_is_valid         (name_t handle) { return kb_is_valid_idx(handle.idx); }           \
  static inline kb_handle_idx kb_to_arr           (name_t handle) { return handle.idx - 1; }                        \
  static inline name_t        name_t##_from_arr   (uint32_t idx)  { return { idx + 1 }; }                           \
  static inline bool          operator==          (const name_t& a, const name_t& b)  { return a.idx == b.idx; }    \
  static inline bool          operator!=          (const name_t& a, const name_t& b)  { return !(a == b); }         \

#define INVALID_HANDLE_CHECK(name, handle) if (!kb_is_valid(handle)) kb_log_error("Using invalid {} handle", #name);
