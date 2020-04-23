
#pragma once

#include "Math/Vector4.hpp"
#include "Math/MathFunctions.hpp"
#include "CoreFlags.hpp"

//----------------------------------------------------------------------------- 
//  LERP - linear interpolation
// 
//  This function returns a point on a line segment given in parametric form. 
//  Where the inputs are point a (starting point), point b (ending point) of  
//  the line segment and the parametric variable t.  If t is in the range of  
//  ranging from t=0.0 to 1.0, this function returns a point on the line      
//  segment. If t=0.0 this function returns point a, if t=1.0 this returns    
//  point b.  Values of t<0.0 return points on the infinite line defined      
//  before point a. Values of t>1.0 returns points on the infinite line       
//  defined after point b.                                                    
// 
//  inputs:
// 		a - point A
// 		b - point B
// 		t - parametric t
// 
//  @return Vector (point) linearly interpolated based on parametric t
// 
//  Example:
// 
//      Vect   v1(1.0f, 2.0f, 3.0f);   // create v1=[1,2,3,1]
//      Vect   v2(-5.0f, 2.0f, 7.0f);  // create v2=[-5,2,7,1]
//      Vect   out;                    // create out vector
// 
//      out =	lineParametric (v1, v2, 0.4f );  // t = 0.4f
// 
//----------------------------------------------------------------------------- 

namespace Math
{
forceinline float GetRadiansBetween(const Vector4 vec0, const Vector4& vec1)
{
	Vector4 v0 = vec0.GetNormalized();
	Vector4 v1 = vec1.GetNormalized();
	float cosAngle = v0.Dot(v1);
	return Math::Acos(cosAngle);
}
// Gets angle between 2 vectors
forceinline float GetDegreesBetween(const Vector4 vec0, const Vector4& vec1)
{
	return Math::RadiansToDegrees(GetRadiansBetween(vec0, vec1));
}

forceinline Vector4 Reflect(const Vector4& v, const Vector4& n)
{
	return v - 2 * v.Dot(n) * n;
}

// 	forceinline Vector4 Refract(const Vector4& incident, const Vector4& normal, float32 indexEta)
// 	{
// 		float32 ni = normal.Dot(incident);
// 		float32 k = 1.f - indexEta * indexEta * (1.f - ni * ni);
// 		return Select(Vector4::Zero, indexEta * incident - (indexEta * ni + Sqrt(k)) * normal, k >= 0);
// 	}

// 	inline void Lerp(Vector4 &out, const Vector4& a, const Vector4& b, const float t)
// 	{
// 		// out = a + t * (b - a);
// 		out = a + t * (b - a);
// 	}

inline void LerpArray(Vector4 *out, const Vector4* a, const Vector4* b, const float t, const int numVects)
{
	for (int i = 0; i < numVects; ++i)
	{
		out[i] = a[i] + t * (b[i] - a[i]);
	}
}

}
