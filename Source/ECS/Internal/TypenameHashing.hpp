#pragma once

#include <type_traits>
#include "Types/Intrinsics.hpp"
#include "String/StringView.hpp"
#include "String/CStringUtilities.hpp"

namespace Musa::Internal
{
template <typename Type>
constexpr forceinline const char* TypenameString()
{
	// TODO - This is a windows specific macro. Need to make a gcc and clang alias for this exact macro somewhere...
	return __FUNCSIG__;
}

namespace
{
constexpr const tchar* compilerTypename = TypenameString<int>();
constexpr size_t standaloneTypeLen = Strlen("int");
constexpr size_t beforeTypeLen = Strstr(compilerTypename, "int") - compilerTypename;
constexpr size_t afterTypeLen = Strlen(compilerTypename) - beforeTypeLen - standaloneTypeLen;
}

template <typename Type>
constexpr forceinline StringView TypeString()
{
	constexpr StringView funcName = TypenameString<Type>();
	return StringView(funcName + beforeTypeLen, Strlen(funcName) - beforeTypeLen - afterTypeLen);
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

