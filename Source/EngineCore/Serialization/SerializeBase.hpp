// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"

class SerializeBase
{
public:
	SerializeBase() = default;
	virtual ~SerializeBase() = default;

	virtual void SerializeData(const void* data, size_t dataSize) = 0;

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
	friend void Serialize(SerializeBase& ser, const tchar* c, size_t size);
	friend void Serialize(SerializeBase& ser, const uint8* u16, size_t size);
	friend void Serialize(SerializeBase& ser, const int8* i16, size_t size);
	friend void Serialize(SerializeBase& ser, const uint16* u16, size_t size);
	friend void Serialize(SerializeBase& ser, const int16* i16, size_t size);
	friend void Serialize(SerializeBase& ser, const uint32* u32, size_t size);
	friend void Serialize(SerializeBase& ser, const int32* i32, size_t size);
	friend void Serialize(SerializeBase& ser, const uint64* u64, size_t size);
	friend void Serialize(SerializeBase& ser, const int64* i64, size_t size);
	friend void Serialize(SerializeBase& ser, const float* f, size_t size);
	friend void Serialize(SerializeBase& ser, const double* d, size_t size);
};
