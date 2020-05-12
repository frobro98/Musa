// Copyright 2020, Nathan Blane
#pragma once

#include "Memory.h"

// forward declaration
class Heap;

// overloaded new/delete
void * operator new( size_t inSize, Heap *inHeap, Memory::Align align, char *inName, int lineNum);
void operator delete( void *p, Heap *inHeap, Memory::Align align, char *inName, int lineNum );

void * operator new[]( size_t inSize, Heap *pHeap, Memory::Align align, char *inName, int lineNum );
void operator delete[]( void *p, Heap *pHeap, Memory::Align align, char *inName, int lineNum );

// Your new system needs to coexist with existing new/delete()
// simple new/delete is used in the unit test fixture.
// welcome to memory system development

void* operator new( size_t inSize );
void operator delete( void *p );

void* operator new[]( size_t inSize );
void operator delete[]( void *p );

