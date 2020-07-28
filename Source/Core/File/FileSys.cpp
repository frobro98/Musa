// Copyright 2020, Nathan Blane

#include "Platform/Platform.hpp"
#include "FileSys.hpp"
#include "Debugging/Assertion.hpp"

unsigned int FileModeToWin32Access(FileMode mode)
{
	switch (mode)
	{
	case FileMode::Read:
		return GENERIC_READ;
	case FileMode::Write:
		return GENERIC_WRITE;
	case FileMode::ReadWrite:
		return GENERIC_READ | GENERIC_WRITE;
	default:
		return 0;
	}
}

unsigned int FileLocationToWin32FP(FileLocation location)
{
	switch (location)
	{
	case FileLocation::Begin:
		return FILE_BEGIN;
	case FileLocation::Current:
		return FILE_CURRENT;
	case FileLocation::End:
		return FILE_END;
	default:
		return 0xffffffff;
	}
}

// TODO - There needs to be some way to pass in flags that dictate the file behavior. This is pretty simplistic
FileResult File::Open( File::Handle &fh, const tchar * const fileName, FileMode mode )
{
	DWORD fileAccess = FileModeToWin32Access(mode);

	fh = CreateFile(
		fileName, 
		fileAccess, 
		FILE_SHARE_READ, 
		nullptr, 
		mode == FileMode::Read ? OPEN_EXISTING : (DWORD)OPEN_ALWAYS,
		mode == FileMode::Read ? FILE_ATTRIBUTE_READONLY : (DWORD)FILE_ATTRIBUTE_NORMAL,
		nullptr
	);

	if (fh == INVALID_HANDLE_VALUE)
	{
		//DWORD err = GetLastError();
		return FileResult::OpenFail;
	}

	return 	FileResult::Success;
}

FileResult File::Close( const File::Handle fh )
{
	if (!CloseHandle(fh))
	{
		return FileResult::CloseFail;
	}

	return 	FileResult::Success;
}

FileResult File::Write( File::Handle fh, const void* const buffer, u32 inSize )
{
	if (buffer == nullptr || inSize == (size_t)-1)
	{
		return FileResult::WriteFail;
	}

	DWORD bytesWritten;
	bool result = WriteFile(fh, buffer, inSize, &bytesWritten, nullptr);
	if (result == false)
	{
		return FileResult::WriteFail;
	}

	return 	FileResult::Success;
}

FileResult File::Read( File::Handle fh,  void* const buffer, u32 inSize )
{
	if (buffer == nullptr)
	{
		return FileResult::ReadFail;
	}

	DWORD bytesRead;
	bool result = ReadFile(fh, buffer, inSize, &bytesRead, nullptr);

	if (result == false)
	{
		return FileResult::ReadFail;
	}

	return FileResult::Success;
}

FileResult File::Size(File::Handle fh, u32& fileSize)
{
	// Assumes file is at the beginning!
	FileResult result = File::Seek(fh, FileLocation::End, 0);
	Assert(result == FileResult::Success);

	result = File::Tell(fh, fileSize);
	Assert(result == FileResult::Success);

	result = File::Seek(fh, FileLocation::Begin, 0);
	return result;
}

FileResult File::Seek( File::Handle fh, FileLocation location, i32 offset )
{
	DWORD moveMethod = FileLocationToWin32FP(location);
	DWORD result = SetFilePointer(fh, offset, nullptr, moveMethod);
	if (result == INVALID_SET_FILE_POINTER)
	{
		return FileResult::SeekFail;
	}

	return 	FileResult::Success;
}

FileResult File::Tell( File::Handle fh, u32 &offset )
{
	offset = SetFilePointer(fh, 0, nullptr, FILE_CURRENT);
	if (offset == INVALID_SET_FILE_POINTER)
	{
		return FileResult::TellFail;
	}

	return FileResult::Success;
}

FileResult File::Flush( File::Handle fh )
{
	bool result = FlushFileBuffers(fh);
	if (result == false)
	{
		return FileResult::FlushFail;
	}

	return 	FileResult::Success;
}

bool File::DoesDirectoryExist(const Path& path)
{
	if (!path.IsEmpty())
	{
		DWORD attrib = GetFileAttributes(path.GetString());
		return attrib != INVALID_FILE_ATTRIBUTES &&
			!!(attrib & FILE_ATTRIBUTE_DIRECTORY);
	}
	return false;
}

bool File::DoesFileExist(const Path& path)
{
	if (!path.IsEmpty())
	{
		DWORD attrib = GetFileAttributes(path.GetString());
		return attrib != INVALID_FILE_ATTRIBUTES &&
			!(attrib & FILE_ATTRIBUTE_DIRECTORY);
	}
	return false;
}

