// Copyright 2020, Nathan Blane

#include "Containers/MemoryBuffer.hpp"

#include "Archiver/PackageHeader.h"
#include "File/FileCore/DirectoryUtilities.h"
#include "File/FileCore/File.h"

#define FILE_CHECK(x, errStr)						\
		do {										\
			if((!(x)))								\
			{										\
				printf(errStr);						\
				exit(1);							\
			}										\
		} while (false)

#define FILE_CHECK_VA(x, errStr, ...)				\
		do {										\
			if((!(x)))								\
			{										\
				printf(errStr, (__VA_ARGS__));		\
				exit(1);							\
			}										\
		} while (false)

//---------------------------------------------------------------------------
// Leave this signature - do your LIU magic here
// Feel free to add files and methods to this project
//---------------------------------------------------------------------------

void PackFilesTogether(char* outputFile, char* packageName, size_t packageNameLen, char* packageVersion, size_t versionLen)
{
	DirectoryDescription dirDesc = GetAllFileTypesInCurrentDirectory("*.chnk");

	u32 packageFileSize = 0;
	PackageHeader header = {};
	header.numChunks = dirDesc.numberOfFiles;
	memcpy(header.packageName, packageName, packageNameLen);
	memcpy(header.versionString, packageVersion, versionLen);


	File::Handle outFileHandle;
	File::Result result = File::Result::SUCCESS;
	result = File::Open(outFileHandle, outputFile, File::Mode::WRITE);
	FILE_CHECK(result == File::Result::SUCCESS, "Output file couldn't be opened!");

	result = File::Write(outFileHandle, &header, sizeof(PackageHeader));
	FILE_CHECK(result == File::Result::SUCCESS, "Output file couldn't be written to!");

	for (u32 i = 0; i < dirDesc.numberOfFiles; ++i)
	{
		const char* file = *dirDesc.files[i];
		File::Handle fileHandle;
		result = File::Open(fileHandle, file, File::Mode::READ);
		FILE_CHECK(result == File::Result::SUCCESS, "Intermediate File couldn't be opened");

		u32 fileSize;
		File::Seek(fileHandle, File::Location::END, 0);
		File::Tell(fileHandle, fileSize);
		File::Seek(fileHandle, File::Location::BEGIN, 0);
		FILE_CHECK_VA(fileSize > 0, "%s doesn't exist!\n", file);
		packageFileSize += fileSize;

		MemoryBuffer fileData(fileSize);
		result = File::Read(fileHandle, fileData.GetData(), fileSize * sizeof(u8));
		FILE_CHECK(result == File::Result::SUCCESS, "Intermediate File couldn't be read");

		result = File::Close(fileHandle);
		FILE_CHECK(result == File::Result::SUCCESS, "Intermediate File couldn't be closed");

		result = File::Write(outFileHandle, fileData.GetData(), fileSize * sizeof(u8));
		FILE_CHECK(result == File::Result::SUCCESS, "Output file couldn't be written to!");
	}

	header.totalSize = packageFileSize;

	File::Seek(outFileHandle, File::Location::BEGIN, 0);
	File::Write(outFileHandle, &header, sizeof(PackageHeader));
	FILE_CHECK(result == File::Result::SUCCESS, "Output file couldn't be written to!");

	File::Close(outFileHandle);
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("You must pass in arguments into this program!\n");
		return -1;
	}

	char* outputFile = nullptr;
	char* packageName = nullptr;
	size_t packageNameLen = 0;
	char* packageVersion = nullptr;
	size_t versionLen = 0;
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp("-v", argv[i]) == 0 || strcmp("-V", argv[i]) == 0)
		{
			++i;
			char* s = strchr(argv[i], '-');
			if (i >= argc || (s != nullptr && strlen(s) == 2))
			{
				printf("You must pass a correct type when using the -n option!\n");
				return -1;
			}

			packageVersion = argv[i];
			versionLen = strlen(packageVersion);
			if (versionLen >= PackageVersionSize)
			{
				printf("The string you passed in for the version is too long! It must be <= %d characters!\n", PackageVersionSize);
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

			packageName = argv[i];
			packageNameLen = strlen(packageName);
			if (packageNameLen >= PackageNameSize)
			{
				printf("The string you passed in for the name is too long! It must be <= %d characters!\n", PackageNameSize);
				return -1;
			}
		}
		else if (strchr(argv[i], '.'))
		{
			outputFile = argv[i];
		}
		else
		{
			printf("Unrecognized argument '%s'\n", argv[i]);
			return -1;
		}
	}

	if (outputFile == nullptr)
	{
		printf("You must pass in a file to write the output!\n");
		return -1;
	}
	if (packageName == nullptr)
	{
		printf("You must pass in a package name using -n!\n");
		return -1;
	}
	if (packageVersion == nullptr)
	{
		printf("You must pass in a package version using -v!\n");
		return -1;
	}

	PackFilesTogether(outputFile, packageName, packageNameLen, packageVersion, versionLen);

	return 0;
}

// End of file
