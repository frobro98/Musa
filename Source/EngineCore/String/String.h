#pragma once

#include "Platform.h"
#include "Containers/DynamicArray.hpp"

#include "fmt/format.h"

class SerializeBase;
class DeserializeBase;

// TODO - add functions to convert numbers to strings
class String
{
public:
	String() = default;
	String(const String& strObj) = default;
	String(String&& strObj) noexcept = default;
	String& operator=(const String& strObj) = default;
	String& operator=(String&& strObj) noexcept = default;

	explicit String(const tchar* cStr, uint32 size);
	String(const tchar* cStr);

	String& operator=(const tchar* cStr);

	uint32 Length() const;
	String Trim() const;
	void Replace(const tchar* toFind, const tchar* toReplace);
	String SubStr(uint32 startIndex) const;
	String SubStr(uint32 startIndex, uint32 endIndex) const;
	DynamicArray<String> Split(const tchar* charToSplitOn) const;
	int32 IndexOf(tchar ch) const;
	tchar CharAt(uint32 index) const;
	int32 FindFirst(const tchar* str) const;
	int32 FindLast(const tchar* str) const;
	int32 FindRange(uint32 startIndex, uint32 endIndex, const tchar* str) const;
	int32 FindFrom(uint32 index, const tchar* str) const;

	void Insert(tchar c, uint32 index);
	void Remove(uint32 index, uint32 count = 1);
	void RemoveAll(tchar c);

	bool StartsWith(const tchar* cStr) const;
	bool StartsWith(tchar ch) const;
	bool EndsWith(const tchar* cStr) const;
	bool EndsWith(tchar ch) const;
	bool IsEmpty() const;
	bool Contains(const tchar* str) const;

	String GetUpperCase() const;
	void ToUpperCase();
	String GetLowerCase() const;
	void ToLowerCase();

	int32 Compare(const String& str) const;
	int32 Compare(const tchar* str) const;
	int32 Compare(const String& str, uint32 numToCompare) const;
	int32 Compare(const tchar* str, uint32 numToCompare) const;

public:
	// TODO - Determine whether this makes sense to have in the class or have it as a function somewhere else
	template<typename... StrArgs>
	static String Format(const tchar* formatStr, StrArgs... args)
	{
		fmt::memory_buffer buf;
		fmt::format_to(buf, formatStr, args...);
		return String(buf.data(), (uint32)buf.size());
	}

public:
	tchar operator[](uint32 index) const;
	const tchar* operator*() const;

	String& operator+=(const String& strObj);
	String& operator+=(const tchar* strObj);

	friend String operator+(const String& str0, const String& str1);
	friend String operator+(const String& str0, const tchar* str1);
	friend String operator+(const tchar* str0, const String& str1);

private:
	DynamicArray<tchar> stringData;

public:
	// Boolean operators
	friend bool operator==(const String& left, const String& right);
	friend bool operator!=(const String& left, const String& right);
	friend bool operator>(const String& left, const String& right);
	friend bool operator<(const String& left, const String& right);
	friend bool operator>=(const String& left, const String& right);
	friend bool operator<=(const String& left, const String& right);

	// C-String comparisons
	friend bool operator==(const String& left, const tchar* right);
	friend bool operator!=(const String& left, const tchar* right);
	friend bool operator>(const String& left, const tchar* right);
	friend bool operator<(const String& left, const tchar* right);
	friend bool operator>=(const String& left, const tchar* right);
	friend bool operator<=(const String& left, const tchar* right);

	friend bool operator==(const tchar* left, const String& right);
	friend bool operator!=(const tchar* left, const String& right);
	friend bool operator>(const tchar* left, const String& right);
	friend bool operator<(const tchar* left, const String& right);
	friend bool operator>=(const tchar* left, const String& right);
	friend bool operator<=(const tchar* left, const String& right);

	friend uint32 GetHash(const String& str);
	friend void Serialize(SerializeBase& ser, const String& str);
	friend void Deserialize(DeserializeBase& ser, String& str);
};
