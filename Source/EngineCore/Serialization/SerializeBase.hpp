// Copyright 2020, Nathan Blane

#pragma once

#include "Types/Intrinsics.hpp"

class SerializeBase
{
public:
	SerializeBase() = default;
	virtual ~SerializeBase() = default;

	virtual void SerializeData(const void* data, uint32 dataSize) = 0;

	friend void Serialize(SerializeBase& ser, tchar c);
	friend void Serialize(SerializeBase& ser, uint8 u16);
	friend void Serialize(SerializeBase& ser, int8 i16);
	friend void Serialize(SerializeBase& ser, uint16 u16);
	friend void Serialize(SerializeBase& ser, int16 i16);
	friend void Serialize(SerializeBase& ser, uint32 u32);
	friend void Serialize(SerializeBase& ser, int32 i32);
	friend void Serialize(SerializeBase& ser, uint64 u64);
	friend void Serialize(SerializeBase& ser, int64 i64);
	friend void Serialize(SerializeBase& ser, float f);
	friend void Serialize(SerializeBase& ser, double d);

	// Raw pointer data serialization
	friend void Serialize(SerializeBase& ser, const tchar* c, uint32 size);
	friend void Serialize(SerializeBase& ser, const uint8* u16, uint32 size);
	friend void Serialize(SerializeBase& ser, const int8* i16, uint32 size);
	friend void Serialize(SerializeBase& ser, const uint16* u16, uint32 size);
	friend void Serialize(SerializeBase& ser, const int16* i16, uint32 size);
	friend void Serialize(SerializeBase& ser, const uint32* u32, uint32 size);
	friend void Serialize(SerializeBase& ser, const int32* i32, uint32 size);
	friend void Serialize(SerializeBase& ser, const uint64* u64, uint32 size);
	friend void Serialize(SerializeBase& ser, const int64* i64, uint32 size);
	friend void Serialize(SerializeBase& ser, const float* f, uint32 size);
	friend void Serialize(SerializeBase& ser, const double* d, uint32 size);
};
