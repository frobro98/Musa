// Copyright 2020, Nathan Blane

#pragma once

#include "Types/Intrinsics.hpp"

class DeserializeBase
{
public:
	DeserializeBase() = default;
	virtual ~DeserializeBase() = default;

	virtual void DeserializeData(void* data, size_t dataSize) = 0;

	friend void Deserialize(DeserializeBase& ser, tchar& c);
	friend void Deserialize(DeserializeBase& ser, uint8& u16);
	friend void Deserialize(DeserializeBase& ser, int8& i16);
	friend void Deserialize(DeserializeBase& ser, uint16& u16);
	friend void Deserialize(DeserializeBase& ser, int16& i16);
	friend void Deserialize(DeserializeBase& ser, uint32& u32);
	friend void Deserialize(DeserializeBase& ser, int32& i32);
	friend void Deserialize(DeserializeBase& ser, uint64& u64);
	friend void Deserialize(DeserializeBase& ser, int64& i64);
	friend void Deserialize(DeserializeBase& ser, float& f);
	friend void Deserialize(DeserializeBase& ser, double& d);

	friend void Deserialize(DeserializeBase& ser, tchar*& c, size_t& size);
	friend void Deserialize(DeserializeBase& ser, uint8*& u16, size_t& size);
	friend void Deserialize(DeserializeBase& ser, int8*& i16, size_t& size);
	friend void Deserialize(DeserializeBase& ser, uint16*& u16, size_t& size);
	friend void Deserialize(DeserializeBase& ser, int16*& i16, size_t& size);
	friend void Deserialize(DeserializeBase& ser, uint32*& u32, size_t& size);
	friend void Deserialize(DeserializeBase& ser, int32*& i32, size_t& size);
	friend void Deserialize(DeserializeBase& ser, uint64*& u64, size_t& size);
	friend void Deserialize(DeserializeBase& ser, int64*& i64, size_t& size);
	friend void Deserialize(DeserializeBase& ser, float*& f, size_t& size);
	friend void Deserialize(DeserializeBase& ser, double*& d, size_t& size);
};