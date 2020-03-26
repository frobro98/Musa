#pragma once

#include "PlatformDefinitions.h"

// C-string functions

// TODO - will probably need to revisit for unicode
size_t Strlen(const tchar* str) noexcept;
tchar* Strcpy(tchar* dest, size_t destSize, const tchar* src);
int32 Strcmp(const tchar* str1, const tchar* str2) noexcept;
int32 Strncmp(const tchar* str1, const tchar* str2, size_t count) noexcept;
tchar* Strcat(tchar* dest, size_t sizeDest, const tchar* src, size_t sizeSrc) noexcept;

template<size_t sizeDest>
tchar* Strcat(tchar (&dest)[sizeDest], const tchar* src, uint32 sizeSrc) noexcept
{
	return Strcat(dest, sizeDest, src, sizeSrc);
}

const tchar* Strchr(const tchar* str, tchar character) noexcept;
const tchar* Strrchr(const tchar* str, tchar character) noexcept;
const tchar* Strstr(const tchar* str, const tchar* findStr) noexcept;
tchar* Strstr(tchar* str, const tchar* findStr) noexcept;

int32 FindFirstIn(const tchar* str, size_t strLen, const tchar* findStr, size_t findStrLen);
int32 FindLastIn(const tchar* str, size_t strLen, const tchar* findStr, size_t findStrLen);

bool StartsWith(const tchar* str, size_t strLen, const tchar* startStr, size_t startStrLen);
bool EndsWith(const tchar* str, size_t strLen, const tchar* endStr, size_t endStrLen);

tchar ToUpper(tchar character) noexcept;
tchar ToLower(tchar character) noexcept;
bool IsAlpha(tchar character) noexcept;
bool IsAlnum(tchar character) noexcept;

