#pragma once

#include "Types/Intrinsics.hpp"

namespace Math
{
union BitManip_64
{
	float64 floatVal;
	int64 intVal;
};

union BitManip_32
{
	float32 floatVal;
	int32 intVal;
};

forceinline int32 AsInt32(float32 val)
{
	BitManip_32 u;
	u.intVal = 0;
	u.floatVal = val;

	return u.intVal;
}

forceinline uint32 AsUint32(float32 val)
{
	return (uint32)AsInt32(val);
}

forceinline float32 AsFloat32(int32 val)
{
	BitManip_32 u;
	u.floatVal = 0;
	u.intVal = val;

	return u.floatVal;
}

forceinline float32 AsFloat32(uint32 val)
{
	return AsFloat32((int32)val);
}

forceinline int64 AsInt64(float64 val)
{
	BitManip_64 u;
	u.intVal = 0;
	u.floatVal = val;

	return u.intVal;
}

forceinline uint64 AsUint64(float64 val)
{
	return (uint64)AsInt64(val);
}

forceinline float64 AsFloat64(int64 val)
{
	BitManip_64 u;
	u.floatVal = 0;
	u.intVal = val;

	return u.floatVal;
}

forceinline float64 AsFloat64(uint64 val)
{
	return AsFloat64((int64)val);
}


}

