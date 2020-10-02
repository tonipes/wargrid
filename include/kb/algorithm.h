#pragma once

#include <kb/api.h>
#include <kb/types.h>

#ifdef __cplusplus
extern "C" {
#endif

KB_API_INLINE uint32_t kb_algorithm_select_unique(uint32_t* src, uint32_t* dst, size_t count) {
  uint32_t p = 0;
  for (uint32_t c = 0; c < count; c++) {
    bool found = false;
    for (uint32_t i = 0; i < p; i++) {
      if (dst[i] == src[c]) { found = true; break; }
    }
    if (!found) { dst[p++] = src[c]; }
  }
  return p;
}

#ifdef __cplusplus
}
#endif