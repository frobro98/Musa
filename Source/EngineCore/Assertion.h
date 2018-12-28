#pragma once

#include "Platform.h"

namespace Debug
{
	namespace Internal
	{
		template<class T>
		constexpr bool internalExpressionCheck(const T& exprCheck)
		{
			return (exprCheck) ? true : false;
		}

		template <>
		constexpr bool internalExpressionCheck(const bool& boolExpr)
		{
			return boolExpr;
		}
	}

	void AssertionFailed(const tchar* expr, const tchar* file, int32 line, const tchar* desc);
	void AssertionFailedWithDescription(const tchar* expr, const tchar* file, int32 line, const tchar* desc, ...);
}

#define AssertStr(x, str)																\
		do																		\
		{																		\
			if(!Debug::Internal::internalExpressionCheck(x))					\
				Debug::AssertionFailed(#x, __FILE__, __LINE__, str);					\
																				\
		} while(false)

#define Assert(x) AssertStr(x, "")

#define Assertf(x, str, ...)																\
		do																		\
		{																		\
			if(!Debug::Internal::internalExpressionCheck(x))					\
				Debug::AssertionFailedWithDescription(#x, __FILE__, __LINE__, str, ##__VA_ARGS__);		\
																				\
		} while(false)