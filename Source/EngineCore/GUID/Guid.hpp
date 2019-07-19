#pragma once

#include <type_traits>

#include "Types/Intrinsics.hpp"
#include "MemoryUtilities.h"
#include "String/String.h"

constexpr uint32 GuidSize = 16;

class Guid
{
public:
	Guid();
	Guid(const uint8* data);
	Guid(const Guid& other);
	Guid& operator=(const Guid&);

	String ToString();
	inline uint32 GetHash() const
	{
		return hash;
	}

	friend bool operator==(const Guid& lhs, const Guid& rhs)
	{
		return Memcmp(lhs.bytes, rhs.bytes, GuidSize) == 0;
	}

	friend bool operator!=(const Guid& lhs, const Guid& rhs)
	{
		return Memcmp(lhs.bytes, rhs.bytes, GuidSize) != 0;
	}

	friend bool operator<(const Guid& lhs, const Guid& rhs)
	{
		return Memcmp(lhs.bytes, rhs.bytes, GuidSize) < 0;
	}

	friend bool operator<=(const Guid& lhs, const Guid& rhs)
	{
		return Memcmp(lhs.bytes, rhs.bytes, GuidSize) <= 0;
	}

	friend bool operator>(const Guid& lhs, const Guid& rhs)
	{
		return Memcmp(lhs.bytes, rhs.bytes, GuidSize) > 0;
	}

	friend bool operator>=(const Guid& lhs, const Guid& rhs)
	{
		return Memcmp(lhs.bytes, rhs.bytes, GuidSize) >= 0;
	}

private:
	void ComputeHash();

private:
	uint8 bytes[GuidSize];
	uint32 hash;
};
