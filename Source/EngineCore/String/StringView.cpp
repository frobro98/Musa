#include "StringView.hpp"
#include "Assertion.h"
#include "CStringUtilities.hpp"

StringView::StringView(const tchar* str)
	: string(str),
	stringLen(Strlen(str))
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
	uint32 searchStrLen = Strlen(str);
	return FindFirstIn(string + startIndex, endIndex - startIndex, str, searchStrLen);
}

int32 StringView::FindFrom(uint32 index, const tchar* str) const
{
	Assert(str);
	Assert(index < Length());
	uint32 searchStrLen = Strlen(str);
	return FindFirstIn(string + index, stringLen - index, str, searchStrLen);
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
