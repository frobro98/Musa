
#include "String/String.h"
#include "FNV-1a.h"
#include "MemoryUtilities.h"
#include "CStringUtilities.hpp"
#include "Assertion.h"

String::String(const tchar* cStr)
{
	Assert(cStr);
	uint32 len = Strlen(cStr);
	if (len > 0)
	{
		stringData.AddDefault(len + 1);
		Memcpy(stringData.GetData(), len * sizeof(tchar), cStr, len * sizeof(tchar));
	}
}

String& String::operator=(const tchar* cStr)
{
	Assert(cStr);
	uint32 len = Strlen(cStr);
	if (len > 0)
	{
		stringData.AddDefault(len + 1);
		Memcpy(stringData.GetData(), len * sizeof(tchar), cStr, len * sizeof(tchar));
	} 

	return *this;
}

String::String(const tchar* cStr, uint32 size)
{
	Assert(cStr);
	if (size > 0)
	{
		uint32 adjustedSize = size + 1;
		stringData.AddDefault(adjustedSize);
		Memcpy(stringData.GetData(), size * sizeof(tchar), cStr, size * sizeof(tchar));
	}
}

uint32 String::Length() const
{
	return !IsEmpty() ? stringData.Size() - 1 : 0;
}

String String::Trim() const
{
	uint32 numWhitespace = 0;
	const tchar* beginning = stringData.GetData();
	const tchar* ending = beginning + stringData.Size();
	Assert(*ending == '\0');
	--ending;
	while (*beginning == ' ')
	{
		++numWhitespace;
		++beginning;
	}

	while (*ending == ' ')
	{
		++numWhitespace;
		--ending;
	}


	return String(beginning, stringData.Size() - numWhitespace);
}

void String::Replace(const tchar* toFind, const tchar* toReplace)
{
	Assert(toFind);
	Assert(toReplace);

	if (!IsEmpty() && *toFind)
	{
		uint32 findLen = Strlen(toFind);
		uint32 replaceLen = Strlen(toReplace);

		if (findLen == replaceLen)
		{
			tchar* str = Strstr(stringData.GetData(), toFind);
			while (str != nullptr)
			{
				for (uint32 i = 0; i < replaceLen; ++i)
				{
					str[i] = toReplace[i];
				}

				str = Strstr(str, toFind);
			}
		}
		else
		{
			String me(*this);
			stringData.Clear();

			tchar* original = me.stringData.GetData();
			tchar* str = Strstr(original, toFind);
			while (str != nullptr)
			{
				*str = '\0';
				(*this) += original;
				(*this) += toReplace;
				original = str + 1;
				str = Strstr(original, toFind);
			}
			(*this) += original;
		}
	}
}

String String::SubStr(uint32 startIndex) const
{
	Assert(startIndex < stringData.Size());
	return String(stringData.GetData() + startIndex, stringData.Size() - startIndex);
}

String String::SubStr(uint32 startIndex, uint32 endIndex) const
{
	Assert(startIndex < stringData.Size());
	Assert(endIndex < stringData.Size() + 1);
	uint32 diff = startIndex + (stringData.Size() - endIndex);
	return String(stringData.GetData() + startIndex, stringData.Size() - diff);
}

DynamicArray<String> String::Split(const tchar* charToSplitOn) const
{
	// TODO - Implement string split
	UNUSED(charToSplitOn);
	return DynamicArray<String>();
}

int32 String::IndexOf(tchar ch) const
{
	int32 indexOf = 0;
	while (stringData[indexOf] != '\0')
	{
		if (stringData[indexOf] == ch)
		{
			return indexOf;
		}
		++indexOf;
	}

	return -1;
}

tchar String::CharAt(uint32 index) const
{
	Assert(index < stringData.Size());
	return stringData[index];
}

int32 String::FindFirst(const tchar* str) const
{
	return FindFirstIn(stringData.GetData(), Length(), str, Strlen(str));
}

int32 String::FindLast(const tchar* str) const
{
	return FindLastIn(stringData.GetData(), Length(), str, Strlen(str));
}

int32 String::FindRange(uint32 startIndex, uint32 endIndex, const tchar* str) const
{
	Assert(str);
	Assert(startIndex < stringData.Size());
	Assert(startIndex < endIndex);
	Assert(endIndex <= stringData.Size());
	uint32 searchStrLen = Strlen(str);
	return FindFirstIn(stringData.GetData() + startIndex, endIndex - startIndex, str, searchStrLen);
}

int32 String::FindFrom(uint32 index, const tchar* str) const
{
	Assert(str);
	Assert(index < Length());
	uint32 searchStrLen = Strlen(str);
	return FindFirstIn(stringData.GetData() + index, Length() - index, str, searchStrLen);
}

void String::Insert(tchar c, uint32 index)
{
	stringData.Insert(c, index);
}

void String::Remove(uint32 index, uint32 count)
{
	Assert(index + count <= Length());
	stringData.Remove(index, count);
}

void String::RemoveAll(tchar c)
{
	stringData.RemoveAll(c);
}

bool String::StartsWith(const tchar* cStr) const
{
	return ::StartsWith(stringData.GetData(), Length(), cStr, Strlen(cStr));
}

bool String::StartsWith(tchar ch) const
{
	return stringData[0] == ch;
}

bool String::EndsWith(const tchar* cStr) const
{
	return ::EndsWith(stringData.GetData(), Length(), cStr, Strlen(cStr));
}

bool String::EndsWith(tchar ch) const
{
	// account for null terminator
	return stringData[stringData.Size() - 2] == ch;
}

bool String::IsEmpty() const
{
	return stringData.Size() == 0;
}

bool String::Contains(const tchar* str) const
{
	return Strstr(stringData.GetData(), str);
}

String String::GetUpperCase() const
{
	String upper(*this);
	upper.ToUpperCase();
	return upper;
}

void String::ToUpperCase()
{
	for (uint32 i = 0; i < Length(); ++i)
	{
		stringData[i] = ToUpper(stringData[i]);
	}
}

String String::GetLowerCase() const
{
	String lower(*this);
	lower.ToLowerCase();
	return lower;
}

void String::ToLowerCase()
{
	for (uint32 i = 0; i < Length(); ++i)
	{
		stringData[i] = ToLower(stringData[i]);
	}
}

int32 String::Compare(const String& str) const
{
	return Strcmp(stringData.GetData(), str.stringData.GetData());
}

int32 String::Compare(const tchar* str) const
{
	return Strcmp(stringData.GetData(), str);
}

int32 String::Compare(const String& str, uint32 numToCompare) const
{
	return Strncmp(**this, *str, numToCompare);
}

int32 String::Compare(const tchar* str, uint32 numToCompare) const
{
	return Strncmp(**this, str, numToCompare);
}

tchar String::operator[](uint32 index) const
{
	Assert(index < stringData.Size());
	return stringData[index];
}

String& String::operator+=(const String& strObj)
{
	uint32 oldLen = stringData.Size();
	stringData.Resize(oldLen + strObj.Length());
	Strcat(stringData.GetData(), stringData.Size() + 1, strObj.stringData.GetData(), strObj.stringData.Size());

	return *this;
}

String& String::operator+=(const tchar* strObj)
{
	uint32 oldLen = Length();
	uint32 cStrLen = Strlen(strObj);
	stringData.Resize(oldLen + cStrLen + 1);
	Strcat(stringData.GetData(), stringData.Size(), strObj, cStrLen);

	return *this;
}

const tchar* String::operator*() const
{
	return stringData.GetData();
}

String operator+(const String& str0, const String& str1)
{
	String s(str0);
	s += str1;
	return s;
}

String operator+(const String& str0, const tchar* str1)
{
	String s(str0);
	s += str1;
	return s;
}

String operator+(const tchar* str0, const String& str1)
{
	String s(str0);
	s += str1;
	return s;
}

bool operator==(const String& left, const String& right)
{
	return left.Compare(right) == 0;
}

bool operator!=(const String& left, const String& right)
{
	return left.Compare(right) != 0;
}

bool operator>(const String& left, const String& right)
{
	return left.Compare(right) > 0;
}

bool operator<(const String& left, const String& right)
{
	return left.Compare(right) < 0;
}

bool operator>=(const String& left, const String& right)
{
	return left.Compare(right) >= 0;
}

bool operator<=(const String& left, const String& right)
{
	return left.Compare(right) <= 0;
}

bool operator==(const String& left, const tchar* right)
{
	return left.Compare(right) == 0;
}

bool operator!=(const String& left, const tchar* right)
{
	return left.Compare(right) != 0;
}

bool operator>(const String& left, const tchar* right)
{
	return left.Compare(right) > 0;
}

bool operator<(const String& left, const tchar* right)
{
	return left.Compare(right) < 0;
}

bool operator>=(const String& left, const tchar* right)
{
	return left.Compare(right) >= 0;
}

bool operator<=(const String& left, const tchar* right)
{
	return left.Compare(right) <= 0;
}

bool operator==(const tchar* left, const String& right)
{
	return Strcmp(left, right.stringData.GetData()) == 0;
}

bool operator!=(const tchar* left, const String& right)
{
	return Strcmp(left, right.stringData.GetData()) != 0;
}

bool operator>(const tchar* left, const String& right)
{
	return Strcmp(left, right.stringData.GetData()) > 0;
}

bool operator<(const tchar* left, const String& right)
{
	return Strcmp(left, right.stringData.GetData()) < 0;
}

bool operator>=(const tchar* left, const String& right)
{
	return Strcmp(left, right.stringData.GetData()) >= 0;
}

bool operator<=(const tchar* left, const String& right)
{
	return Strcmp(left, right.stringData.GetData()) <= 0;
}

//////////////////////////////////////////////////////////////////////////
// String hash definition
//////////////////////////////////////////////////////////////////////////

uint32 GetHash(const String& str)
{
	return fnv(str.stringData.GetData(), static_cast<uint32>(str.stringData.Size()));
}

void Serialize(SerializeBase& ser, const String& str)
{
	Serialize(ser, str.Length());
	for (uint32 i = 0; i < str.Length(); ++i)
	{
		Serialize(ser, str.stringData[i]);
	}
}

void Deserialize(DeserializeBase& ser, String& str)
{
	uint32 stringLen;
	Deserialize(ser, stringLen);

	str.stringData.Resize(stringLen + 1);
	for (uint32 i = 0; i < stringLen; ++i)
	{
		Deserialize(ser, str.stringData[i]);
	}
}
