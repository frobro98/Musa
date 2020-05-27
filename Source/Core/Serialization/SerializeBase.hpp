// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "CoreAPI.hpp"

class CORE_API SerializeBase
{
public:
	SerializeBase() = default;
	virtual ~SerializeBase() = default;

	virtual void SerializeData(const void* data, size_t dataSize) = 0;

	friend CORE_API void Serialize(SerializeBase& ser, tchar c);
	friend CORE_API void Serialize(SerializeBase& ser, u8 u16);
	friend CORE_API void Serialize(SerializeBase& ser, i8 i16);
	friend CORE_API void Serialize(SerializeBase& ser, u16 u16);
	friend CORE_API void Serialize(SerializeBase& ser, i16 i16);
	friend CORE_API void Serialize(SerializeBase& ser, u32 u32);
	friend CORE_API void Serialize(SerializeBase& ser, i32 i32);
	friend CORE_API void Serialize(SerializeBase& ser, u64 u64);
	friend CORE_API void Serialize(SerializeBase& ser, i64 i64);
	friend CORE_API void Serialize(SerializeBase& ser, float f);
	friend CORE_API void Serialize(SerializeBase& ser, double d);

	// Raw pointer data serialization
	friend CORE_API void Serialize(SerializeBase& ser, const tchar* c, size_t size);
	friend CORE_API void Serialize(SerializeBase& ser, const u8* u16, size_t size);
	friend CORE_API void Serialize(SerializeBase& ser, const i8* i16, size_t size);
	friend CORE_API void Serialize(SerializeBase& ser, const u16* u16, size_t size);
	friend CORE_API void Serialize(SerializeBase& ser, const i16* i16, size_t size);
	friend CORE_API void Serialize(SerializeBase& ser, const u32* u32, size_t size);
	friend CORE_API void Serialize(SerializeBase& ser, const i32* i32, size_t size);
	friend CORE_API void Serialize(SerializeBase& ser, const u64* u64, size_t size);
	friend CORE_API void Serialize(SerializeBase& ser, const i64* i64, size_t size);
	friend CORE_API void Serialize(SerializeBase& ser, const float* f, size_t size);
	friend CORE_API void Serialize(SerializeBase& ser, const double* d, size_t size);
};
