#pragma once

#include "BasicTypes/Intrinsics.hpp"
#include "CoreAPI.hpp"


NODISCARD void* operator new(size_t count);
NODISCARD void* operator new(size_t count, const std::nothrow_t&) noexcept;
NODISCARD void* operator new(size_t count, std::align_val_t al);
NODISCARD void* operator new(size_t count, std::align_val_t al, const std::nothrow_t&) noexcept;
NODISCARD void* operator new[](size_t count);
NODISCARD void* operator new[](size_t count, const std::nothrow_t&) noexcept;
NODISCARD void* operator new[](size_t count, std::align_val_t al);
NODISCARD void* operator new[](size_t count, std::align_val_t al, const std::nothrow_t&) noexcept;
void operator delete(void* p) throw();
void operator delete(void* p, const std::nothrow_t&) noexcept;
void operator delete(void* p, size_t size) throw();
void operator delete(void* p, size_t size, const std::nothrow_t&) noexcept;
void operator delete[](void* p) throw();
void operator delete[](void* p, const std::nothrow_t&) noexcept;
void operator delete[](void* p, size_t size) throw();
void operator delete[](void* p, size_t size, const std::nothrow_t&) noexcept;

