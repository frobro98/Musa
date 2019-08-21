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
	if (str != nullptr)
	{
		const tchar* ptr = str;
		while (*ptr != '\0')
		{
			++ptr;
			++len;
		}
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

int32 FindFirstIn(const tchar* str, uint32 strLen, const tchar* findStr, uint32 findStrLen)
{
	Assert(str);
	Assert(findStr);
	if (findStrLen == 0 || findStrLen > strLen)
	{
		return -1;
	}

	for (const tchar* iter = str;; ++iter)
	{
		iter = Strstr(str, findStr);
		if (iter == nullptr)
		{
			return -1;
		}
		uint32 iterLen = strLen - static_cast<uint32>(iter - str);
		uint32 count = std::min(iterLen, findStrLen);
		if (Strncmp(iter, findStr, count) == 0)
		{
			return static_cast<int32>(iter - str);
		}
	}
}

int32 FindLastIn(const tchar* str, uint32 strLen, const tchar* findStr, uint32 findStrLen)
{
	Assert(str);
	Assert(findStr);
	if (findStrLen == 0 || findStrLen > strLen)
	{
		return -1;
	}

	const tchar* const endPos = str + strLen;
	for (const tchar* iter = endPos - findStrLen; iter != (str - 1); --iter)
	{
		uint32 i;
		for (i = 0; i < findStrLen; ++i)
		{
			if (iter[i] != findStr[i])
			{
				break;
			}
		}

		if (findStr[i] == '\0')
		{
			return static_cast<int32>(iter - str);
		}
	}

	return -1;
}

bool StartsWith(const tchar* str, uint32 strLen, const tchar* startStr, uint32 startStrLen)
{
	if (startStrLen > strLen)
	{
		return false;
	}

	while (*startStr != '\0')
	{
		if (*startStr != *str)
		{
			return false;
		}
		++str;
		++startStr;
	}
	return true;
}

bool EndsWith(const tchar* str, uint32 strLen, const tchar* endStr, uint32 endStrLen)
{
	if (endStrLen > strLen)
	{
		return false;
	}

	// account for null terminator
	const tchar* strData = str + strLen - 1;
	const tchar* endStrData = endStr + endStrLen - 1;
	int32 chCount = static_cast<int32>(endStrLen);

	while (chCount > 0)
	{
		if (*strData != *endStrData)
		{
			return false;
		}

		--endStrData;
		--strData;
		--chCount;
	}
	return true;
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
	if (str1 == nullptr)
	{
		return -1;
	}
	else if (str2 == nullptr)
	{
		return 1;
	}

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
