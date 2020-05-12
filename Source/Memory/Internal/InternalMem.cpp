// Copyright 2020, Nathan Blane

#include "InternalMem.h"

Block * InternalMem::GetGlobalHead() const
{
	return pGlobalHead;
}

void InternalMem::SetGlobalHead(Block * block)
{
	pGlobalHead = block;
}
