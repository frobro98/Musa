// Copyright 2020, Nathan Blane

#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "Memory/MemoryFunctions.hpp"

#define GLOBAL_OPERATOR_NEW_DELETE_OVERLOADS \
	NODISCARD void* operator new(size_t count) { return Memory::Malloc(count); } \
	NODISCARD void* operator new(size_t count, const std::nothrow_t&) noexcept { return Memory::Malloc(count); } \
	NODISCARD void* operator new(size_t count, std::align_val_t al) { return Memory::Malloc(count, (size_t)al); } \
	NODISCARD void* operator new(size_t count, std::align_val_t al, const std::nothrow_t &) noexcept { return Memory::Malloc(count, (size_t)al); } \
	NODISCARD void* operator new[](size_t count) { return Memory::Malloc(count); } \
	NODISCARD void* operator new[](size_t count, const std::nothrow_t&) noexcept { return Memory::Malloc(count); } \
	NODISCARD void* operator new[](size_t count, std::align_val_t al) { return Memory::Malloc(count, (size_t)al); } \
	NODISCARD void* operator new[](size_t count, std::align_val_t al, const std::nothrow_t &) noexcept { return Memory::Malloc(count, (size_t)al); } \
	void operator delete(void* p) throw() { return Memory::Free(p); } \
	void operator delete(void* p, const std::nothrow_t&) noexcept { return Memory::Free(p); } \
	void operator delete(void* p, size_t /*size*/) throw() { return Memory::Free(p); } \
	void operator delete(void* p, size_t /*size*/, const std::nothrow_t&) noexcept { return Memory::Free(p); } \
	void operator delete[](void* p) throw() { return Memory::Free(p); } \
	void operator delete[](void* p, const std::nothrow_t&) noexcept { return Memory::Free(p); } \
	void operator delete[](void* p, size_t /*size*/) throw() { return Memory::Free(p); } \
	void operator delete[](void* p, size_t /*size*/, const std::nothrow_t&) noexcept { return Memory::Free(p); }

