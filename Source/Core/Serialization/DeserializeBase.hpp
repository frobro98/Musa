// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "CoreAPI.hpp"

class CORE_API DeserializeBase
{
public:
	DeserializeBase() = default;
	virtual ~DeserializeBase() = default;

	virtual void DeserializeData(void* data, size_t dataSize) = 0;

	friend CORE_API void Deserialize(DeserializeBase& ser, tchar& c);
	friend CORE_API void Deserialize(DeserializeBase& ser, u8& u16);
	friend CORE_API void Deserialize(DeserializeBase& ser, i8& i16);
	friend CORE_API void Deserialize(DeserializeBase& ser, u16& u16);
	friend CORE_API void Deserialize(DeserializeBase& ser, i16& i16);
	friend CORE_API void Deserialize(DeserializeBase& ser, u32& u32);
	friend CORE_API void Deserialize(DeserializeBase& ser, i32& i32);
	friend CORE_API void Deserialize(DeserializeBase& ser, u64& u64);
	friend CORE_API void Deserialize(DeserializeBase& ser, i64& i64);
	friend CORE_API void Deserialize(DeserializeBase& ser, float& f);
	friend CORE_API void Deserialize(DeserializeBase& ser, double& d);

	friend CORE_API void Deserialize(DeserializeBase& ser, tchar*& c, size_t& size);
	friend CORE_API void Deserialize(DeserializeBase& ser, u8*& u16, size_t& size);
	friend CORE_API void Deserialize(DeserializeBase& ser, i8*& i16, size_t& size);
	friend CORE_API void Deserialize(DeserializeBase& ser, u16*& u16, size_t& size);
	friend CORE_API void Deserialize(DeserializeBase& ser, i16*& i16, size_t& size);
	friend CORE_API void Deserialize(DeserializeBase& ser, u32*& u32, size_t& size);
	friend CORE_API void Deserialize(DeserializeBase& ser, i32*& i32, size_t& size);
	friend CORE_API void Deserialize(DeserializeBase& ser, u64*& u64, size_t& size);
	friend CORE_API void Deserialize(DeserializeBase& ser, i64*& i64, size_t& size);
	friend CORE_API void Deserialize(DeserializeBase& ser, float*& f, size_t& size);
	friend CORE_API void Deserialize(DeserializeBase& ser, double*& d, size_t& size);
};