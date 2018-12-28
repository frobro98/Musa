#pragma once

#include "Platform.h"
#include "String/String.h"
#include "Containers/Array.h"

class Skeleton;

class SkeletonManager
{
public:
	static Skeleton* CreateSkeleton(uint8* skeletonData, uint32 hash);
	static void RemoveSkeleton(Skeleton* skeleton);
	static Skeleton* FindSkeleton(uint32 hash);

private:
	void AddSkeleton(Skeleton* skeleton);

	SkeletonManager() = default;
	~SkeletonManager() = default;

	static SkeletonManager& Instance();

private:
	Array<Skeleton*> skeletons;

};