#pragma once

#include "Types/Intrinsics.hpp"

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

	friend uint32 GetHash(const StringView& str);
};
