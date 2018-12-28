//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 
// FILEIO v.2.1.0
//----------------------------------------------------------------------------- 

#include "EngineCore/Platform.h"

WALL_WRN_PUSH
#include <stdio.h>
#include <assert.h>
#include <string.h>
WALL_WRN_POP

// locals
#include "FileIO.h"


FileIO::FileIO()
	:fileHandle(0)
{
};

FileIO::~FileIO()
{
	if (0 != fileHandle)
	{
		this->privClose();
	}
};

void FileIO::Open(const char * const pFirstName, const char * const pLastName)
{
	assert(pFirstName);
	assert(pLastName);
	privGetInstance()->privOpen(pFirstName, pLastName);
}

void FileIO::Close()
{
	privGetInstance()->privClose();
}

FILE *FileIO::GetHandle()
{
	return privGetInstance()->privGetHandle();
}

FileIO *FileIO::privGetInstance()
{
	static FileIO instance;
	return &instance;
}

void FileIO::privClose()
{
	errno_t fileError;
	assert(fileHandle);

	fileError = fflush(fileHandle);
	assert(!fileError);

	fileError = fclose(fileHandle);
	fileHandle = 0;
	assert(!fileError);
}

void FileIO::privOpen(const char * const pFirstName, const char * const pLastName)
{
	const int length = 256;
	
	char *pFileName = new char[length];
	assert(pFileName);

	errno_t fileError;
	char *out_string = "Output";

	// wash the name to 0
	memset(pFileName, 0, length);
	assert(strlen(pFirstName) < length/4);
	assert(strlen(pLastName) < length/4);

	strcat_s(pFileName, length, pFirstName);
	strcat_s(pFileName, length, pLastName);
	strcat_s(pFileName, length, out_string);

#ifdef _DEBUG
	char *debug_string = "Debug.txt";
	strcat_s(pFileName, length, debug_string);
	char *mode = "DEBUG";
#else
	char *debug_string = "Release.txt";
	strcat_s(pFileName, length, debug_string);
	char *mode = "RELEASE";
#endif

	fileError = fopen_s(&fileHandle, pFileName, "wt");
	assert(fileHandle);

	fprintf(fileHandle, "Program Assignment: 2 \n");
	fprintf(fileHandle, "Name: %s %s\n", pFirstName, pLastName);
	fprintf(fileHandle, "Mode: %s\n", mode);
	fprintf(fileHandle, "\n");
	fprintf(fileHandle, "-------------------------------------------------\n");
	fprintf(fileHandle, "\n");

	delete pFileName;
}


FILE *FileIO::privGetHandle()
{
	assert(fileHandle);
	return fileHandle;
}

// ---  End of File ---------------

