#include "StringView.hpp"
#include "Assertion.h"
#include "CStringUtilities.hpp"
#include "String.h"
#include "FNV-1a.h"

StringView::StringView(const tchar* str)
	: string(str),
	stringLen((uint32)Strlen(str))
{
	Assert(string != nullptr);
}

StringView::StringView(const tchar* str, uint32 len)
	: string(str),
	stringLen(len)
{
	Assert(string != nullptr);
	Assert(Strlen(str) >= len);
}

StringView StringView::SubStr(uint32 startIndex) const
{
	Assert(startIndex < stringLen);
	return StringView(string + startIndex);
}

StringView StringView::SubStr(uint32 startIndex, uint32 endIndex) const
{
	Assert(startIndex < stringLen);
	Assert(startIndex < endIndex);
	Assert(endIndex <= stringLen);
	return StringView(string + startIndex, endIndex - startIndex);
}

bool StringView::StartsWith(const tchar* cStr) const
{
	return ::StartsWith(string, stringLen, cStr, Strlen(cStr));
}

bool StringView::StartsWith(tchar ch) const
{
	return string[0] == ch;
}

bool StringView::EndsWith(const tchar* cStr) const
{
	return ::EndsWith(string, stringLen, cStr, Strlen(cStr));
}

bool StringView::EndsWith(tchar ch) const
{
	return string[stringLen - 1] == ch;
}

int32 StringView::FindFirst(const tchar* str) const
{
	return FindFirstIn(string, stringLen, str, Strlen(str));
}

int32 StringView::FindLast(const tchar* str) const
{
	return FindLastIn(string, stringLen, str, Strlen(str));
}

int32 StringView::FindRange(uint32 startIndex, uint32 endIndex, const tchar* str) const
{
	Assert(str);
	Assert(startIndex < stringLen);
	Assert(startIndex < endIndex);
	Assert(endIndex <= stringLen);
	size_t searchStrLen = Strlen(str);
	int32 foundFirstIndex = FindFirstIn(string + startIndex, endIndex - startIndex, str, searchStrLen);
	return foundFirstIndex + startIndex;
}

int32 StringView::FindFrom(uint32 index, const tchar* str) const
{
	Assert(str);
	Assert(index < Length());
	size_t searchStrLen = Strlen(str);
	int32 foundFirstIndex = FindFirstIn(string + index, stringLen - index, str, searchStrLen);
	return foundFirstIndex + index;
}

bool StringView::IsEmpty() const
{
	return stringLen == 0;
}

bool StringView::Contains(const tchar* str) const
{
	return Strstr(string, str);
}

uint32 StringView::Length() const
{
	return stringLen;
}

int32 StringView::IndexOf(tchar ch) const
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

tchar StringView::CharAt(uint32 index) const
{
	Assert(index < stringLen);
	return string[index];
}

tchar StringView::operator[](uint32 index) const
{
	Assert(index < stringLen);
	return string[index];
}

const tchar* StringView::operator*() const
{
	return string;
}

int32 StringView::Compare(const StringView& sv) const
{
	return Strcmp(string, sv.string);
}

int32 StringView::Compare(const String& s) const
{
	return Strcmp(string, *s);
}

int32 StringView::Compare(const tchar* cs) const
{
	return Strcmp(string, cs);
}

int32 StringView::Compare(const StringView& sv, uint32 compLen) const
{
	return Strncmp(string, sv.string, compLen);
}

int32 StringView::Compare(const String& s, uint32 compLen) const
{
	return Strncmp(string, *s, compLen);
}

int32 StringView::Compare(const tchar* cs, uint32 compLen) const
{
	return Strncmp(string, cs, compLen);
}

uint32 GetHash(const StringView& str)
{
	return fnv32(str.string, str.stringLen);
}

bool operator==(const StringView& s0, const StringView& s1)
{
	return s0.Compare(s1) == 0;
}

bool operator==(const String& s, const StringView& sv)
{
	return sv.Compare(s) == 0;
}

bool operator==(const StringView& sv, const String& s)
{
	return sv.Compare(s) == 0;
}

bool operator==(const tchar* cs, const StringView& sv)
{
	return sv.Compare(cs) == 0;
}

bool operator==(const StringView& sv, const tchar* cs)
{
	return sv.Compare(cs) == 0;
}

bool operator!=(const StringView& s0, const StringView& s1)
{
	return s0.Compare(s1) != 0;
}

bool operator!=(const String& s, const StringView& sv)
{
	return sv.Compare(s) != 0;
}

bool operator!=(const StringView& sv, const String& s)
{
	return sv.Compare(s) != 0;
}

bool operator!=(const tchar* cs, const StringView& sv)
{
	return sv.Compare(cs) != 0;
}

bool operator!=(const StringView& sv, const tchar* cs)
{
	return sv.Compare(cs) != 0;
}
