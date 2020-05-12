// Copyright 2020, Nathan Blane

#pragma once

#include "Types/Intrinsics.hpp"
#include "Assertion.h"
#include "CStringUtilities.hpp"
#include "String.h"
#include "Utilities/Hash.hpp"

class String;

class StringView
{
public:
	constexpr StringView(const tchar* str);
	constexpr explicit StringView(const tchar* str, uint32 len);

	constexpr StringView SubStr(uint32 startIndex) const;
	constexpr StringView SubStr(uint32 startIndex, uint32 endIndex) const;

	constexpr bool StartsWith(const tchar* cStr) const;
	constexpr bool StartsWith(tchar ch) const;
	constexpr bool EndsWith(const tchar* cStr) const;
	constexpr bool EndsWith(tchar ch) const;
	constexpr int32 FindFirst(const tchar* str) const;
	constexpr int32 FindLast(const tchar* str) const;
	constexpr int32 FindRange(uint32 startIndex, uint32 endIndex, const tchar* str) const;
	constexpr int32 FindFrom(uint32 index, const tchar* str) const;

	constexpr bool IsEmpty() const;
	constexpr bool Contains(const tchar* str) const;
	constexpr uint32 Length() const;

	constexpr int32 IndexOf(tchar ch) const;
	constexpr tchar CharAt(uint32 index) const;
	constexpr tchar operator[](uint32 index) const;

	constexpr const tchar* operator*() const;

	constexpr int32 Compare(const StringView& sv) const;
	constexpr int32 Compare(const String& s) const;
	constexpr int32 Compare(const tchar* cs) const;
	constexpr int32 Compare(const StringView& sv, uint32 compLen) const;
	constexpr int32 Compare(const String& s, uint32 compLen) const;
	constexpr int32 Compare(const tchar* cs, uint32 compLen) const;

	friend constexpr bool operator==(const StringView& s0, const StringView& s1);
	friend constexpr bool operator==(const String& s, const StringView& sv);
	friend constexpr bool operator==(const StringView& sv, const String& s);
	friend constexpr bool operator==(const tchar* cs, const StringView& sv);
	friend constexpr bool operator==(const StringView& sv, const tchar* cs);

	friend constexpr bool operator!=(const StringView& s0, const StringView& s1);
	friend constexpr bool operator!=(const String& s, const StringView& sv);
	friend constexpr bool operator!=(const StringView& sv, const String& s);
	friend constexpr bool operator!=(const tchar* cs, const StringView& sv);
	friend constexpr bool operator!=(const StringView& sv, const tchar* cs);

private:
	const tchar* string;
	uint32 stringLen;

	friend uint32 GetHash(const StringView& str)
	{
		return fnv32(str.string, str.stringLen);
	}
};

constexpr StringView::StringView(const tchar* str)
	: string(str),
	stringLen((uint32)Strlen(str))
{
	Assert(string != nullptr);
}

constexpr StringView::StringView(const tchar* str, uint32 len)
	: string(str),
	stringLen(len)
{
	Assert(string != nullptr);
	Assert(Strlen(str) >= len);
}

constexpr StringView StringView::SubStr(uint32 startIndex) const
{
	Assert(startIndex < stringLen);
	return StringView(string + startIndex);
}

constexpr StringView StringView::SubStr(uint32 startIndex, uint32 endIndex) const
{
	Assert(startIndex < stringLen);
	Assert(startIndex < endIndex);
	Assert(endIndex <= stringLen);
	return StringView(string + startIndex, endIndex - startIndex);
}

constexpr bool StringView::StartsWith(const tchar* cStr) const
{
	return ::StartsWith(string, stringLen, cStr, Strlen(cStr));
}

constexpr bool StringView::StartsWith(tchar ch) const
{
	return string[0] == ch;
}

constexpr bool StringView::EndsWith(const tchar* cStr) const
{
	return ::EndsWith(string, stringLen, cStr, Strlen(cStr));
}

constexpr bool StringView::EndsWith(tchar ch) const
{
	return string[stringLen - 1] == ch;
}

constexpr int32 StringView::FindFirst(const tchar* str) const
{
	return FindFirstIn(string, stringLen, str, Strlen(str));
}

constexpr int32 StringView::FindLast(const tchar* str) const
{
	return FindLastIn(string, stringLen, str, Strlen(str));
}

constexpr int32 StringView::FindRange(uint32 startIndex, uint32 endIndex, const tchar* str) const
{
	Assert(str);
	Assert(startIndex < stringLen);
	Assert(startIndex < endIndex);
	Assert(endIndex <= stringLen);
	size_t searchStrLen = Strlen(str);
	int32 foundFirstIndex = FindFirstIn(string + startIndex, endIndex - startIndex, str, searchStrLen);
	return foundFirstIndex + startIndex;
}

constexpr int32 StringView::FindFrom(uint32 index, const tchar* str) const
{
	Assert(str);
	Assert(index < Length());
	size_t searchStrLen = Strlen(str);
	int32 foundFirstIndex = FindFirstIn(string + index, stringLen - index, str, searchStrLen);
	return foundFirstIndex + index;
}

constexpr bool StringView::IsEmpty() const
{
	return stringLen == 0;
}

constexpr bool StringView::Contains(const tchar* str) const
{
	return Strstr(string, str);
}

constexpr uint32 StringView::Length() const
{
	return stringLen;
}

constexpr int32 StringView::IndexOf(tchar ch) const
{
	int32 indexOf = 0;
	while (string[indexOf] != '\0')
	{
		if (string[indexOf] == ch)
		{
			return indexOf;
		}
		++indexOf;
	}

	return -1;
}

constexpr tchar StringView::CharAt(uint32 index) const
{
	Assert(index < stringLen);
	return string[index];
}

constexpr tchar StringView::operator[](uint32 index) const
{
	Assert(index < stringLen);
	return string[index];
}

constexpr const tchar* StringView::operator*() const
{
	return string;
}

constexpr int32 StringView::Compare(const StringView& sv) const
{
	return Strcmp(string, sv.string);
}

constexpr int32 StringView::Compare(const String& s) const
{
	return Strcmp(string, *s);
}

constexpr int32 StringView::Compare(const tchar* cs) const
{
	return Strcmp(string, cs);
}

constexpr int32 StringView::Compare(const StringView& sv, uint32 compLen) const
{
	return Strncmp(string, sv.string, compLen);
}

constexpr int32 StringView::Compare(const String& s, uint32 compLen) const
{
	return Strncmp(string, *s, compLen);
}

constexpr int32 StringView::Compare(const tchar* cs, uint32 compLen) const
{
	return Strncmp(string, cs, compLen);
}

constexpr bool operator==(const StringView& s0, const StringView& s1)
{
	return s0.Compare(s1) == 0;
}

constexpr bool operator==(const String& s, const StringView& sv)
{
	return sv.Compare(s) == 0;
}

constexpr bool operator==(const StringView& sv, const String& s)
{
	return sv.Compare(s) == 0;
}

constexpr bool operator==(const tchar* cs, const StringView& sv)
{
	return sv.Compare(cs) == 0;
}

constexpr bool operator==(const StringView& sv, const tchar* cs)
{
	return sv.Compare(cs) == 0;
}

constexpr bool operator!=(const StringView& s0, const StringView& s1)
{
	return s0.Compare(s1) != 0;
}

constexpr bool operator!=(const String& s, const StringView& sv)
{
	return sv.Compare(s) != 0;
}

constexpr bool operator!=(const StringView& sv, const String& s)
{
	return sv.Compare(s) != 0;
}

constexpr bool operator!=(const tchar* cs, const StringView& sv)
{
	return sv.Compare(cs) != 0;
}

constexpr bool operator!=(const StringView& sv, const tchar* cs)
{
	return sv.Compare(cs) != 0;
}
