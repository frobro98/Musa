// Copyright 2020, Nathan Blane

#include "DeserializeBase.hpp"

void Deserialize(DeserializeBase& ser, tchar& c)
{
	ser.DeserializeData(&c, sizeof(tchar));
}

void Deserialize(DeserializeBase& ser, u8& u8)
{
	ser.DeserializeData(&u8, sizeof(u8));
}

void Deserialize(DeserializeBase& ser, i8& i8)
{
	ser.DeserializeData(&i8, sizeof(i8));
}

void Deserialize(DeserializeBase& ser, u16& u16)
{
	ser.DeserializeData(&u16, sizeof(u16));
}

void Deserialize(DeserializeBase& ser, i16& i16)
{
	ser.DeserializeData(&i16, sizeof(i16));
}

void Deserialize(DeserializeBase& ser, u32& u32)
{
	ser.DeserializeData(&u32, sizeof(u32));
}

void Deserialize(DeserializeBase& ser, i32& i32)
{
	ser.DeserializeData(&i32, sizeof(i32));
}

void Deserialize(DeserializeBase& ser, u64& u64)
{
	ser.DeserializeData(&u64, sizeof(u64));
}

void Deserialize(DeserializeBase& ser, i64& i64)
{
	ser.DeserializeData(&i64, sizeof(i64));
}

void Deserialize(DeserializeBase& ser, float& f)
{
	ser.DeserializeData(&f, sizeof(float));
}

void Deserialize(DeserializeBase& ser, double& d)
{
	ser.DeserializeData(&d, sizeof(double));
}

void Deserialize(DeserializeBase& ser, tchar*& c, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(u32));
	c = new tchar[size];
	ser.DeserializeData(c, size);
}

void Deserialize(DeserializeBase& ser, u8*& u8, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	u8 = new u8[size];
	ser.DeserializeData(u8, size);
}

void Deserialize(DeserializeBase& ser, i8*& i8, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	i8 = new i8[size];
	ser.DeserializeData(i8, size);
}

void Deserialize(DeserializeBase& ser, u16*& u16, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	u16 = new u16[size];
	ser.DeserializeData(u16, size * sizeof(u16));
}

void Deserialize(DeserializeBase& ser, i16*& i16, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	i16 = new i16[size];
	ser.DeserializeData(i16, size * sizeof(i16));
}

void Deserialize(DeserializeBase& ser, u32*& u32, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	u32 = new u32[size];
	ser.DeserializeData(u32, size * sizeof(u32));
}

void Deserialize(DeserializeBase& ser, i32*& i32, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	i32 = new i32[size];
	ser.DeserializeData(i32, size * sizeof(i32));
}

void Deserialize(DeserializeBase& ser, u64*& u64, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	u64 = new u64[size];
	ser.DeserializeData(u64, size * sizeof(u64));
}

void Deserialize(DeserializeBase& ser, i64*& i64, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	i64 = new i64[size];
	ser.DeserializeData(i64, size * sizeof(i64));
}

void Deserialize(DeserializeBase& ser, float*& f, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	f = new float[size];
	ser.DeserializeData(f, size * sizeof(float));
}

void Deserialize(DeserializeBase& ser, double*& d, size_t& size)
{
	// TODO - This allocates memory when the memory should be already allocated honestly...
	ser.DeserializeData(&size, sizeof(size_t));
	d = new double[size];
	ser.DeserializeData(d, size * sizeof(double));
}



