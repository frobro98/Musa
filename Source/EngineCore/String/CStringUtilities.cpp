#include "CStringUtilities.hpp"
#include "Assertion.h"

void EvaluateFormattedString(tchar * buffer, uint32 bufferLen, const tchar * format, va_list vaList)
{
	vsnprintf(buffer, bufferLen, format, vaList);
	va_end(vaList);
}

void FormatString(tchar * buffer, uint32 bufferLen, const tchar * format, ...)
{
	va_list args = nullptr;
	EvaluateFormattedString(buffer, bufferLen, format, args);
}

uint32 Strlen(const tchar* str) noexcept
{
	uint32 len = 0;
	const tchar* ptr = str;
	while (*ptr != '\0')
	{
		++ptr;
		++len;
	}
	return len;
}

tchar* Strcpy(tchar* dest, size_t destSize, const tchar* src)
{
	strcpy_s(dest, destSize, src);
	return dest;
}

tchar* Strcat(tchar* dest, uint32 sizeDest, const tchar* src, uint32 sizeSrc) noexcept
{
	uint32 destStrLen = Strlen(dest);
	Assert(sizeDest >= sizeSrc + 1);
	Assert(destStrLen <= (sizeDest - sizeSrc) + 1);

	tchar* destPtr = dest + destStrLen;
	while (*src != '\0')
	{
		*destPtr = *src;
		++src;
		++destPtr;
	}
	*destPtr = '\0';

	return dest;
}

const tchar* Strchr(const tchar* str, tchar character) noexcept
{
	return ::strchr(str, character);
}

const tchar* Strrchr(const tchar* str, tchar character) noexcept
{
	return ::strrchr(str, character);
}

const tchar* Strstr(const tchar* str, const tchar* findStr) noexcept
{
	return strstr(str, findStr);
}

tchar* Strstr(tchar* str, const tchar* findStr) noexcept
{
	return strstr(str, findStr);
}

tchar ToUpper(tchar character) noexcept
{
	return (tchar)::toupper(character);
}

tchar ToLower(tchar character) noexcept
{
	return (tchar)::tolower(character);
}

bool IsAlpha(tchar character) noexcept
{
	return isalpha(character);
}

bool IsAlnum(tchar character) noexcept
{
	return isalnum(character);
}

int32 Strcmp(const tchar* str1, const tchar* str2) noexcept
{
	uint32 len1 = Strlen(str1);
	uint32 len2 = Strlen(str2);
	uint32 minLen = std::min(len1, len2);
	for (uint32 i = 0; i < minLen + 1; ++i, ++str1, ++str2)
	{
		if (*str1 != *str2)
		{
			return *str1 < *str2 ? -1 : 1;
		}
	}
	return 0;
}

int32 Strncmp(const tchar* str1, const tchar* str2, uint32 count) noexcept
{
	uint32 len1 = Strlen(str1);
	uint32 len2 = Strlen(str2);
	uint32 minLen = std::min(len1, len2);
	Assert(count <= minLen);
	for (uint32 i = 0; i < count; ++i, ++str1, ++str2)
	{
		if (*str1 != *str2)
		{
			return *str1 < *str2 ? -1 : 1;
		}
	}
	return 0;
}
