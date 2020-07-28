// Copyright 2020, Nathan Blane

#include <stdio.h>
#include <string.h>

#include "Containers/MemoryBuffer.hpp"

#include "Archiver/ChunkHeader.h"
#include "File/md5.h"
#include "File/FileSystem.hpp"

#define FILE_CHECK(x, errStr)		\
		do {						\
			if((!(x)))				\
			{						\
				printf(errStr);		\
				exit(-1);			\
			}						\
		} while (false)

//---------------------------------------------------------------------------
// Leave this signature - do your Vodka magic here
// Feel free to add files and methods to this project
//---------------------------------------------------------------------------

bool StrToChunkType(const char* type, Chunk& chunkType)
{
	if (strcmp("VERTS_TYPE", type) == 0)
	{
		chunkType = Chunk::VERTS_TYPE;
	}
	else if (strcmp("ANIM_TYPE", type) == 0)
	{
		chunkType = Chunk::ANIM_TYPE;
	}
	else if (strcmp("TEXTURE_TYPE", type) == 0)
	{
		chunkType = Chunk::TEXTURE_TYPE;
	}
	else if (strcmp("SKEL_TYPE", type) == 0)
	{
		chunkType = Chunk::SKEL_TYPE;
	}
	else if (strcmp("WEIGHTS_TYPE", type) == 0)
	{
		chunkType = Chunk::WEIGHTS_TYPE;
	}
	else
	{
		return false;
	}

	return true;
}

void CreateFileWithHeader(const char* inputFile, const char* outputFile, Chunk chunkType, const char* name, size_t nameLen)
{
	FileSystem::Handle fHandle;
	FileSystem::Handle fHandleOutput;
	bool result = FileSystem::OpenFile(fHandle, inputFile, FileMode::Read);
	FILE_CHECK(result, "Input file doesn't exist!\n");

	u32 fileSize;
	FileSystem::SeekFile(fHandle, FileLocation::End, 0);
	fileSize = FileSystem::TellFile(fHandle);
	result = FileSystem::SeekFile(fHandle, FileLocation::Begin, 0);
	FILE_CHECK(fileSize > 0, "Input file doesn't exist!\n");

	MemoryBuffer fileData(fileSize);
	result = FileSystem::ReadFile(fHandle, fileData.GetData(), fileSize * sizeof(u8));
	FILE_CHECK(result, "Input file reading error!\n");

	result = FileSystem::CloseFile(fHandle);
	FILE_CHECK(result, "Input file closing error!\n");

	ChunkHeader header = {};
	header.type = chunkType;
	header.chunkSize = fileSize;
	memcpy(header.chunkName, name, nameLen);

	MD5Output output = {};
	MD5Buffer(fileData.GetData(), static_cast<u32>(fileSize), output);
	header.hashNum = output.dWord_3 ^ output.dWord_2 ^ output.dWord_1 ^ output.dWord_0;

	result = FileSystem::OpenFile(fHandleOutput, outputFile, FileMode::Write);
	FILE_CHECK(result, "Output file couldn't be opened!\n");

	result = FileSystem::WriteFile(fHandleOutput, &header, sizeof(ChunkHeader));
	FILE_CHECK(result, "Output file writing error!\n");

	result = FileSystem::WriteFile(fHandleOutput, fileData.GetData(), fileSize * sizeof(u8));
	FILE_CHECK(result, "Output file writing error!\n");

	FileSystem::CloseFile(fHandleOutput);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("You must pass in arguments into this program!\n");
		return -1;
	}

	char* inputFile = nullptr;
	char* outputFile = nullptr;
	Chunk type = static_cast<Chunk>(-1);
	char* chunkName = nullptr;
	size_t chunkNameLen = 0;
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp("-t", argv[i]) == 0 || strcmp("-T", argv[i]) == 0)
		{
			++i;
			if (i >= argc || !StrToChunkType(argv[i], type))
			{
				printf("You must pass a correct type when using the -t option!\n");
				return -1;
			}
		}
		else if (strcmp("-n", argv[i]) == 0 || strcmp("-N", argv[i]) == 0)
		{
			++i;
			char* s = strchr(argv[i], '-');
			if (i >= argc || (s != nullptr && strlen(s) == 2))
			{
				printf("You must pass a correct type when using the -n option!\n");
				return -1;
			}

			chunkName = argv[i];
			chunkNameLen = strlen(chunkName);
			if (chunkNameLen >= ChunkNameSize)
			{
				printf("The string you passed in for the name is too long! It must be <= %d characters!\n", ChunkNameSize);
				return -1;
			}
		}
		else if (strchr(argv[i], '.'))
		{
			if (inputFile == nullptr)
			{
				inputFile = argv[i];
			}
			else
			{
				outputFile = argv[i];
			}
		}
		else
		{
			printf("Unrecognized argument '%s'\n %d\n", argv[i], i);
			return -1;
		}
	}

	if (inputFile == nullptr)
	{
		printf("You must pass in a file to read in as input!\n");
		return -1;
	}
	if (outputFile == nullptr)
	{
		printf("You must pass in a file to write the output!\n");
		return -1;
	}
	if (chunkName == nullptr)
	{
		printf("You must pass in a name for this data chunk using -n!\n");
		return -1;
	}
	if (type == static_cast<Chunk>(-1))
	{
		printf("You must pass in a valid chunk type using -t!\n");
		return -1;
	}

	CreateFileWithHeader(inputFile, outputFile, type, chunkName, chunkNameLen);

	return 0;
}

// End of File