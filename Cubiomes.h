#pragma once
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>

#define CB_ASRT(X, ...)                                                                            \
	{                                                                                              \
		typeof(X) _cb_val = X;                                                                     \
		if (!_cb_val) {                                                                            \
			if (ctx) {                                                                             \
				snprintf(ctx->errstr, sizeof(ctx->errstr), ""__VA_ARGS__);                         \
			}                                                                                      \
			return NULL;                                                                           \
		}                                                                                          \
	}

#define CB_FREE(X) __attribute__((cleanup(cleanup_free##X))) X
#define CB_FREE_SUPPORT(X)                                                                         \
	static void cleanup_free_##X(X **x)                                                            \
	{                                                                                              \
		if (*x)                                                                                    \
			free(x);                                                                               \
	}

CB_FREE_SUPPORT(uint8_t)
CB_FREE_SUPPORT(uint16_t)
CB_FREE_SUPPORT(uint32_t)
CB_FREE_SUPPORT(uint64_t)
CB_FREE_SUPPORT(int8_t)
CB_FREE_SUPPORT(int16_t)
CB_FREE_SUPPORT(int32_t)
CB_FREE_SUPPORT(int64_t)
