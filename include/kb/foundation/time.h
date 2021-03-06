// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

KB_API int64_t  kb_time_get_raw(void);
KB_API int64_t  kb_time_get_current(void);
KB_API int64_t  kb_time_get_frequency(void);
KB_API float    kb_time(void);

#ifdef __cplusplus
}
#endif
