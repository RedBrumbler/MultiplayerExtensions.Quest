#pragma once

#define MPEX_EXPORT __attribute__((visibility("default")))

#ifdef __cplusplus
#define MPEX_EXPORT_FUNC extern "C" MPEX_EXPORT
#else
#define MPEX_EXPORT_FUNC MPEX_EXPORT
#endif