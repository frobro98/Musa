
#include "InternalMem.h"

Block * InternalMem::GetGlobalHead() const
{
	return pGlobalHead;
}

void InternalMem::SetGlobalHead(Block * block)
{
	pGlobalHead = block;
}
