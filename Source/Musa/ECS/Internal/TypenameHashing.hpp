#pragma once

#include <type_traits>
#include "Types/Intrinsics.hpp"

namespace Musa::Internal
{
template <typename Comp>
constexpr forceinline const char* TypenameString()
{
	// TODO - This is a windows specific macro. Need to make a gcc and clang alias for this exact macro somewhere...
	return __FUNCSIG__;
}

template <typename Comp>
constexpr forceinline uint64 TypenameHash()
{
	using sanitizedType = std::remove_reference_t<std::remove_const_t<Comp>>;
	constexpr const char* typeStr = TypenameString<sanitizedType>();
	constexpr uint64 hash = fnv64(typeStr);
	return hash;
}
}

