// Copyright 2020, Nathan Blane

#include "SerializeBase.hpp"
#include "Debugging/Assertion.hpp"

void Serialize(SerializeBase & ser, tchar c)
{
	ser.SerializeData(&c, sizeof(tchar));
}

void Serialize(SerializeBase& ser, u8 u8)
{
	ser.SerializeData(&u8, sizeof(u8));
}

void Serialize(SerializeBase& ser, i8 i8)
{
	ser.SerializeData(&i8, sizeof(i8));
}

void Serialize(SerializeBase& ser, u16 u16)
{
	ser.SerializeData(&u16, sizeof(u16));
}

void Serialize(SerializeBase& ser, i16 i16)
{
	ser.SerializeData(&i16, sizeof(i16));
}

void Serialize(SerializeBase& ser, u32 u32)
{
	ser.SerializeData(&u32, sizeof(u32));
}

void Serialize(SerializeBase& ser, i32 i32)
{
	ser.SerializeData(&i32, sizeof(i32));
}

void Serialize(SerializeBase& ser, u64 u64)
{
	ser.SerializeData(&u64, sizeof(u64));
}

void Serialize(SerializeBase& ser, i64 i64)
{
	ser.SerializeData(&i64, sizeof(i64));
}

void Serialize(SerializeBase& ser, float f)
{
	ser.SerializeData(&f, sizeof(float));
}

void Serialize(SerializeBase& ser, double d)
{
	ser.SerializeData(&d, sizeof(double));
}

void Serialize(SerializeBase& ser, const tchar* c, size_t size)
{
	Assert(c != nullptr);
	ser.SerializeData(&size, sizeof(size_t));
	ser.SerializeData(c, size);
}

void Serialize(SerializeBase& ser, const u8* u8, size_t size)
{
	Assert(u8 != nullptr);
	ser.SerializeData(&size, sizeof(size_t));
	ser.SerializeData(u8, size);
}

void Serialize(SerializeBase& ser, const i8* i8, size_t size)
{			
	Assert(i8 != nullptr);
	ser.SerializeData(&size, sizeof(size_t));
	ser.SerializeData(i8, size);
}							
							
void Serialize(SerializeBase& ser, const u16* u16, size_t size)
{			
	Assert(u16 != nullptr);
	ser.SerializeData(&size, sizeof(size_t));
	ser.SerializeData(u16, size);
}							
							
void Serialize(SerializeBase& ser, const i16* i16, size_t size)
{			
	Assert(i16 != nullptr);
	ser.SerializeData(&size, sizeof(size_t));
	ser.SerializeData(i16, size);
}							
							
void Serialize(SerializeBase& ser, const u32* u32, size_t size)
{			
	Assert(u32 != nullptr);
	ser.SerializeData(&size, sizeof(size_t));
	ser.SerializeData(u32, size);
}							
							
void Serialize(SerializeBase& ser, const i32* i32, size_t size)
{		
	Assert(i32 != nullptr);
	ser.SerializeData(&size, sizeof(size_t));
	ser.SerializeData(i32, size);
}							
							
void Serialize(SerializeBase& ser, const u64* u64, size_t size)
{		
	Assert(u64 != nullptr);
	ser.SerializeData(&size, sizeof(size_t));
	ser.SerializeData(u64, size);
}							
							
void Serialize(SerializeBase& ser, const i64* i64, size_t size)
{		
	Assert(i64 != nullptr);
	ser.SerializeData(&size, sizeof(size_t));
	ser.SerializeData(i64, size);
}							
							
void Serialize(SerializeBase& ser, const float* f, size_t size)
{			
	Assert(f != nullptr);
	ser.SerializeData(&size, sizeof(size_t));
	ser.SerializeData(f, size);
}							
							
void Serialize(SerializeBase& ser, const double* d, size_t size)
{
	Assert(d != nullptr);
	ser.SerializeData(&size, sizeof(size_t));
	ser.SerializeData(d, size);
}
