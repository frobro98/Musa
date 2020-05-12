// Copyright 2020, Nathan Blane

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

uint32 GetRequestedNumFiles(const char* directory, DynamicArray<String>& directoryFiles, uint32 numFilesRequested)
{
	HANDLE findHandle;
	WIN32_FIND_DATA data;
	uint32 numFound = 0;

	findHandle = FindFirstFile(directory, &data);
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		while (numFound < numFilesRequested)
		{
			directoryFiles[numFound] = data.cFileName;

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

	DynamicArray<String> files(filesInDir);
	GetRequestedNumFiles(directory, files, filesInDir);

	return DirectoryDescription{ std::move(files), filesInDir };
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

static uint32 GetRequestedNumFileTypes(const char* fileType, DynamicArray<String>& directoryFiles, uint32 numFilesRequested)
{
	HANDLE findHandle;
	WIN32_FIND_DATA data;
	uint32 numFound = 0;

	findHandle = FindFirstFile(fileType, &data);
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		while (numFound < numFilesRequested)
		{
			directoryFiles[numFound] = data.cFileName;

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

	DynamicArray<String> files(filesInDir);
	GetRequestedNumFileTypes(fileType, files, filesInDir);

	return DirectoryDescription{ std::move(files), filesInDir };
}
