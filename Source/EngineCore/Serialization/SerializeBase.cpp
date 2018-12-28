#include "SerializeBase.hpp"

void Serialize(SerializeBase & ser, tchar c)
{
	ser.SerializeData(&c, sizeof(tchar));
}

void Serialize(SerializeBase& ser, uint8 u8)
{
	ser.SerializeData(&u8, sizeof(uint8));
}

void Serialize(SerializeBase& ser, int8 i8)
{
	ser.SerializeData(&i8, sizeof(int8));
}

void Serialize(SerializeBase& ser, uint16 u16)
{
	ser.SerializeData(&u16, sizeof(uint16));
}

void Serialize(SerializeBase& ser, int16 i16)
{
	ser.SerializeData(&i16, sizeof(int16));
}

void Serialize(SerializeBase& ser, uint32 u32)
{
	ser.SerializeData(&u32, sizeof(uint32));
}

void Serialize(SerializeBase& ser, int32 i32)
{
	ser.SerializeData(&i32, sizeof(int32));
}

void Serialize(SerializeBase& ser, uint64 u64)
{
	ser.SerializeData(&u64, sizeof(uint64));
}

void Serialize(SerializeBase& ser, int64 i64)
{
	ser.SerializeData(&i64, sizeof(int64));
}

void Serialize(SerializeBase& ser, float f)
{
	ser.SerializeData(&f, sizeof(float));
}

void Serialize(SerializeBase& ser, double d)
{
	ser.SerializeData(&d, sizeof(double));
}