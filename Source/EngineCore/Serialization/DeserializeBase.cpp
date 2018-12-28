#include "DeserializeBase.hpp"

void Deserialize(DeserializeBase& ser, tchar& c)
{
	ser.DeserializeData(&c, sizeof(tchar));
}

void Deserialize(DeserializeBase& ser, uint8& u8)
{
	ser.DeserializeData(&u8, sizeof(uint8));
}

void Deserialize(DeserializeBase& ser, int8& i8)
{
	ser.DeserializeData(&i8, sizeof(int8));
}

void Deserialize(DeserializeBase& ser, uint16& u16)
{
	ser.DeserializeData(&u16, sizeof(uint16));
}

void Deserialize(DeserializeBase& ser, int16& i16)
{
	ser.DeserializeData(&i16, sizeof(int16));
}

void Deserialize(DeserializeBase& ser, uint32& u32)
{
	ser.DeserializeData(&u32, sizeof(uint32));
}

void Deserialize(DeserializeBase& ser, int32& i32)
{
	ser.DeserializeData(&i32, sizeof(int32));
}

void Deserialize(DeserializeBase& ser, uint64& u64)
{
	ser.DeserializeData(&u64, sizeof(uint64));
}

void Deserialize(DeserializeBase& ser, int64& i64)
{
	ser.DeserializeData(&i64, sizeof(int64));
}

void Deserialize(DeserializeBase& ser, float& f)
{
	ser.DeserializeData(&f, sizeof(float));
}

void Deserialize(DeserializeBase& ser, double& d)
{
	ser.DeserializeData(&d, sizeof(double));
}
