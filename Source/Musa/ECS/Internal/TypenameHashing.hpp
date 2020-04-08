#pragma once

#include <type_traits>
#include "Types/Intrinsics.hpp"

namespace Musa::Internal
{
template <typename Comp>
static constexpr const char* TypenameString()
{
	return __func__;
}

template <typename Comp>
static constexpr uint64 TypenameHash()
{
	using sanitizedType = std::remove_reference_t<std::remove_const_t>;
	constexpr uint64 hash = fnv64(TypenameString<sanitizedType>());
	return hash;
}
}