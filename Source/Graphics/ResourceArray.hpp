#pragma once

#include "Containers/DynamicArray.hpp"
#include "MemoryUtilities.h"

struct ResourceArray final
{
	template <typename Res>
	ResourceArray(const DynamicArray<Res>& resCollection);
	~ResourceArray()
	{
		delete resourceBytes;
	}

	const uint32 elementCount;
	const uint32 elementStride;
	const uint32 totalByteSize;
	const uint8* resourceBytes;
};

template<typename Res>
inline ResourceArray::ResourceArray(const DynamicArray<Res>& resCollection)
	: elementCount(resCollection.Size())
	elementStride(sizeof(Res)),
	totalByteSize(resCollection.SizeInBytes())
{
	uint8* collectionBytes = new uint8[totalByteSize];
	Memcpy(collectionBytes, totalByteSize, resCollection.GetData(), totalByteSize);
	resourceBytes = collectionBytes;
}
