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

void Deserialize(DeserializeBase& ser, tchar*& c, uint32& size)
{
	ser.DeserializeData(&size, sizeof(uint32));
	c = new tchar[size];
	ser.DeserializeData(c, size);
}

void Deserialize(DeserializeBase& ser, uint8*& u8, uint32& size)
{
	ser.DeserializeData(&size, sizeof(uint32));
	u8 = new uint8[size];
	ser.DeserializeData(u8, size);
}

void Deserialize(DeserializeBase& ser, int8*& i8, uint32& size)
{
	ser.DeserializeData(&size, sizeof(uint32));
	i8 = new int8[size];
	ser.DeserializeData(i8, size);
}

void Deserialize(DeserializeBase& ser, uint16*& u16, uint32& size)
{
	ser.DeserializeData(&size, sizeof(uint32));
	u16 = new uint16[size];
	ser.DeserializeData(u16, size * sizeof(uint16));
}

void Deserialize(DeserializeBase& ser, int16*& i16, uint32& size)
{
	ser.DeserializeData(&size, sizeof(uint32));
	i16 = new int16[size];
	ser.DeserializeData(i16, size * sizeof(int16));
}

void Deserialize(DeserializeBase& ser, uint32*& u32, uint32& size)
{
	ser.DeserializeData(&size, sizeof(uint32));
	u32 = new uint32[size];
	ser.DeserializeData(u32, size * sizeof(uint32));
}

void Deserialize(DeserializeBase& ser, int32*& i32, uint32& size)
{
	ser.DeserializeData(&size, sizeof(uint32));
	i32 = new int32[size];
	ser.DeserializeData(i32, size * sizeof(int32));
}

void Deserialize(DeserializeBase& ser, uint64*& u64, uint32& size)
{
	ser.DeserializeData(&size, sizeof(uint32));
	u64 = new uint64[size];
	ser.DeserializeData(u64, size * sizeof(uint64));
}

void Deserialize(DeserializeBase& ser, int64*& i64, uint32& size)
{
	ser.DeserializeData(&size, sizeof(uint32));
	i64 = new int64[size];
	ser.DeserializeData(i64, size * sizeof(int64));
}

void Deserialize(DeserializeBase& ser, float*& f, uint32& size)
{
	ser.DeserializeData(&size, sizeof(uint32));
	f = new float[size];
	ser.DeserializeData(f, size * sizeof(float));
}

void Deserialize(DeserializeBase& ser, double*& d, uint32& size)
{
	ser.DeserializeData(&size, sizeof(uint32));
	d = new double[size];
	ser.DeserializeData(d, size * sizeof(double));
}



