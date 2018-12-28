#pragma once


#include "Memory.h"

class Block;

class InternalMem : public Memory
{
public:
	Block* GetGlobalHead() const;
	void SetGlobalHead(Block* block);
};