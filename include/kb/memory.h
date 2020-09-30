#pragma once

#include <kb/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  const void* data;
  uint32_t    size;
} Memory;

#define KB_STATIC_MEMORY(src) { (void*)src, sizeof(src) }

#ifdef __cplusplus
}
#endif
