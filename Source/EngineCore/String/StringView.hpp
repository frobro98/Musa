#pragma once

#include "Types/Intrinsics.hpp"

class StringView
{
public:
	explicit StringView(const tchar* str);
	explicit StringView(const tchar* str, uint32 len);

	StringView SubStr(uint32 startIndex) const;
	StringView SubStr(uint32 startIndex, uint32 endIndex) const;

	bool StartsWith(const tchar* cStr) const;
	bool StartsWith(tchar ch) const;
	bool EndsWith(const tchar* cStr) const;
	bool EndsWith(tchar ch) const;
	int32 FindFirst(const tchar* str) const;
	int32 FindLast(const tchar* str) const;
	int32 FindRange(uint32 startIndex, uint32 endIndex, const tchar* str) const;
	int32 FindFrom(uint32 index, const tchar* str) const;

	bool IsEmpty() const;
	bool Contains(const tchar* str) const;
	uint32 Length() const;

	int32 IndexOf(tchar ch) const;
	tchar CharAt(uint32 index) const;
	tchar operator[](uint32 index) const;

	const tchar* operator*() const;

private:
	const tchar* string;
	uint32 stringLen;

	friend uint32 GetHash(const StringView& str);
};
