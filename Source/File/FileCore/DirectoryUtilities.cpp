
#include "DirectoryUtilities.h"

uint32 NumberOfFilesInDirectory(const char* directory)
{
	HANDLE findHandle;
	WIN32_FIND_DATA data;
	uint32 fileCount = 0;

	findHandle = FindFirstFile(directory, &data);
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		do {
			++fileCount;
		} while (FindNextFile(findHandle, &data));
		FindClose(findHandle);
	}

	return fileCount;
}

uint32 GetRequestedNumFiles(const char* directory, char** directoryFiles, uint32 numFilesRequested)
{
	HANDLE findHandle;
	WIN32_FIND_DATA data;
	uint32 numFound = 0;

	findHandle = FindFirstFile(directory, &data);
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		while (numFound < numFilesRequested)
		{
			size_t strLen = strlen(data.cFileName);
			directoryFiles[numFound] = new char[strLen+1];
			memcpy(directoryFiles[numFound], data.cFileName, strLen + 1);

			++numFound;
			if (!FindNextFile(findHandle, &data))
			{
				break;
			}
		}
		FindClose(findHandle);
	}

	return numFound;
}

DirectoryDescription GetAllFilesInDirectory(const char* directory)
{
	uint32 filesInDir = NumberOfFilesInDirectory(directory);

	char** files = new char*[filesInDir];
	GetRequestedNumFiles(directory, files, filesInDir);

	return DirectoryDescription{ files, filesInDir };
}

uint32 NumberOfFileTypesInCurrentDirectory(const char* fileExt)
{
	HANDLE findHandle;
	WIN32_FIND_DATA data;
	uint32 fileCount = 0;
	DWORD error = 0;

	findHandle = FindFirstFile(fileExt, &data);
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		do {
			++fileCount;
		} while (FindNextFile(findHandle, &data));
		FindClose(findHandle);
		error = GetLastError();
	}

	return fileCount;
}

static uint32 GetRequestedNumFileTypes(const char* fileType, char** directoryFiles, uint32 numFilesRequested)
{
	HANDLE findHandle;
	WIN32_FIND_DATA data;
	uint32 numFound = 0;

	findHandle = FindFirstFile(fileType, &data);
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		while (numFound < numFilesRequested)
		{
			size_t strLen = strlen(data.cFileName);
			directoryFiles[numFound] = new char[strLen + 1];
			memcpy(directoryFiles[numFound], data.cFileName, strLen + 1);

			++numFound;

			if (!FindNextFile(findHandle, &data))
			{
				break;
			}
		}
		FindClose(findHandle);
	}

	return numFound;
}

DirectoryDescription GetAllFileTypesInCurrentDirectory(const char* fileType)
{
	uint32 filesInDir = NumberOfFileTypesInCurrentDirectory(fileType);

	char** files = new char*[filesInDir];
	GetRequestedNumFileTypes(fileType, files, filesInDir);

	return DirectoryDescription{ files, filesInDir };
}
