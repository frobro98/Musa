#pragma once

#include "PlatformDefinitions.h"

#define VA_ARGS_PARSE(format, buffer, bufferLen)						\
		do{																\
			va_list args;												\
			va_start(args, format);										\
			EvaluateFormattedString(buffer, bufferLen, format, args);	\
		} while (false)

// Variable argument string functions
void EvaluateFormattedString(tchar* buffer, uint32 bufferLen, const tchar* format, va_list vaList);

void FormatString(tchar* buffer, uint32 bufferLen, const tchar* format, ...);

// C-string functions

// TODO - will probably need to revisit for unicode
uint32 Strlen(const tchar* str) noexcept;
tchar* Strcpy(tchar* dest, size_t destSize, const tchar* src);
int32 Strcmp(const tchar* str1, const tchar* str2) noexcept;
int32 Strncmp(const tchar* str1, const tchar* str2, uint32 count) noexcept;
tchar* Strcat(tchar* dest, uint32 sizeDest, const tchar* src, uint32 sizeSrc) noexcept;

template<uint32 sizeDest>
tchar* Strcat(tchar (&dest)[sizeDest], const tchar* src, uint32 sizeSrc) noexcept
{
	return Strcat(dest, sizeDest, src, sizeSrc);
}

const tchar* Strchr(const tchar* str, tchar character) noexcept;
const tchar* Strrchr(const tchar* str, tchar character) noexcept;
const tchar* Strstr(const tchar* str, const tchar* findStr) noexcept;
tchar* Strstr(tchar* str, const tchar* findStr) noexcept;

tchar ToUpper(tchar character) noexcept;
tchar ToLower(tchar character) noexcept;
bool IsAlpha(tchar character) noexcept;
bool IsAlnum(tchar character) noexcept;

