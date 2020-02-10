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
	
	template<typename... Args>
	inline void AssertionFailed(const tchar* expr, const tchar* file, int32 line, const tchar* desc, [[maybe_unused]] Args... args)
	{
		UNUSED(expr, file, line, desc);
		DebugBreak();
	}
}

#ifdef _DEBUG
#define AssertStr(x, str)														\
		do																		\
		{																		\
			if(!Debug::Internal::internalExpressionCheck(x))					\
				Debug::AssertionFailed(#x, __FILE__, __LINE__, str);			\
																				\
		}while(false)

#define Assert(x) AssertStr(x, #x)

#define Assertf(x, str, ...)													\
		do																		\
		{																		\
			if(!Debug::Internal::internalExpressionCheck(x))					\
				Debug::AssertionFailed(#x, __FILE__, __LINE__, str, ##__VA_ARGS__);		\
			__assume(x);														\
		} while(false)

#define AssertFunc(func, str)											\
		do																\
		{																\
			if(!(func)())												\
				Debug::AssertionFailed(#func, __FILE__, __LINE__, str);	\
		} while (false)
#else
#define AssertStr(x, str)
#define Assert(x)
#define Assertf(x, str, ...)
#define AssertFunc(func, str)
#endif