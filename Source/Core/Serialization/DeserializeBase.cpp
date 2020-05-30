// Copyright 2020, Nathan Blane

#include "DeserializeBase.hpp"

void Deserialize(DeserializeBase& ser, tchar& c)
{
	ser.DeserializeData(&c, sizeof(tchar));
}

void Deserialize(DeserializeBase& ser, u8& uint8)
{
	ser.DeserializeData(&uint8, sizeof(u8));
}

void Deserialize(DeserializeBase& ser, i8& int8)
{
	ser.DeserializeData(&int8, sizeof(i8));
}

void Deserialize(DeserializeBase& ser, u16& uint16)
{
	ser.DeserializeData(&uint16, sizeof(u16));
}

void Deserialize(DeserializeBase& ser, i16& int16)
{
	ser.DeserializeData(&int16, sizeof(i16));
}

void Deserialize(DeserializeBase& ser, u32& uint32)
{
	ser.DeserializeData(&uint32, sizeof(u32));
}

void Deserialize(DeserializeBase& ser, i32& int32)
{
	ser.DeserializeData(&int32, sizeof(i32));
}

void Deserialize(DeserializeBase& ser, u64& uint64)
{
	ser.DeserializeData(&uint64, sizeof(u64));
}

void Deserialize(DeserializeBase& ser, i64& int64)
{
	ser.DeserializeData(&int64, sizeof(i64));
}

void Deserialize(DeserializeBase& ser, f32& f)
{
	ser.DeserializeData(&f, sizeof(f32));
}

void Deserialize(DeserializeBase& ser, f64& d)
{
	ser.DeserializeData(&d, sizeof(f64));
}

void Deserialize(DeserializeBase& ser, tchar*& c, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(u32));
	c = new tchar[size];
	ser.DeserializeData(c, size);
}

void Deserialize(DeserializeBase& ser, u8*& uint8, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	uint8 = new u8[size];
	ser.DeserializeData(uint8, size);
}

void Deserialize(DeserializeBase& ser, i8*& int8, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	int8 = new i8[size];
	ser.DeserializeData(int8, size);
}

void Deserialize(DeserializeBase& ser, u16*& uint16, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	uint16 = new u16[size];
	ser.DeserializeData(uint16, size * sizeof(u16));
}

void Deserialize(DeserializeBase& ser, i16*& int16, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	int16 = new i16[size];
	ser.DeserializeData(int16, size * sizeof(i16));
}

void Deserialize(DeserializeBase& ser, u32*& uint32, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	uint32 = new u32[size];
	ser.DeserializeData(uint32, size * sizeof(u32));
}

void Deserialize(DeserializeBase& ser, i32*& int32, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	int32 = new i32[size];
	ser.DeserializeData(int32, size * sizeof(i32));
}

void Deserialize(DeserializeBase& ser, u64*& uint64, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	uint64 = new u64[size];
	ser.DeserializeData(uint64, size * sizeof(u64));
}

void Deserialize(DeserializeBase& ser, i64*& int64, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	int64 = new i64[size];
	ser.DeserializeData(int64, size * sizeof(i64));
}

void Deserialize(DeserializeBase& ser, f32*& f, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	f = new float[size];
	ser.DeserializeData(f, size * sizeof(float));
}

void Deserialize(DeserializeBase& ser, f64*& d, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	d = new double[size];
	ser.DeserializeData(d, size * sizeof(double));
}



