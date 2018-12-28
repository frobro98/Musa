#pragma once

#include "Platform.h"

#define ALIGN16_OPERATORS							\
	public:											\
		void* operator new(size_t size)				\
		{											\
			void* p = _aligned_malloc(size, 16);	\
			return p;								\
		}											\
													\
		void* operator new[](size_t size)			\
		{											\
			void* p = _aligned_malloc(size, 16);	\
			return p;								\
		}											\
													\
		void operator delete(void* data)			\
		{											\
			_aligned_free(data);					\
		}											\
													\
		void operator delete[](void* data)			\
		{											\
			_aligned_free(data);					\
		}

class Malloc16
{
public:
	void* operator new(size_t size)
	{
		void* p = _aligned_malloc(size, 16);
		return p;
	}

	void* operator new[](size_t size)
	{
		void* p = _aligned_malloc(size, 16);
		return p;
	}

	void operator delete(void* data)
	{
		_aligned_free(data);
	}

	void operator delete[](void* data)
	{
		_aligned_free(data);
	}
};