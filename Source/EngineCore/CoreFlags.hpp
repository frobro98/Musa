#pragma once

#define M_DEBUG _DEBUG
#define M_RELEASE !M_DEBUG

#define forceinline __forceinline

#define likely(x) (x)
#define unlikely(x) (x)

#define UNUSED(...)						\
	__pragma(warning(push))				\
	__pragma(warning(disable : 4548))	\
	(__VA_ARGS__)						\
	__pragma(warning(pop))

#define NOT_USED [[maybe_unused]]
#define NODISCARD [[nodiscard]]

