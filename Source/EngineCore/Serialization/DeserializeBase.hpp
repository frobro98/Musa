#pragma once

#include "EngineCore/Types.h"

class DeserializeBase
{
public:
	DeserializeBase() = default;
	virtual ~DeserializeBase() = default;

	virtual void DeserializeData(void* data, uint32 dataSize) = 0;

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
};