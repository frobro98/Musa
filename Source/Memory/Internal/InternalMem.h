// Copyright 2020, Nathan Blane

#pragma once

#include "Memory.h"

class Block;

class InternalMem : public Memory
{
public:
	Block* GetGlobalHead() const;
	void SetGlobalHead(Block* block);
};