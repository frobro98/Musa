// Copyright 2020, Nathan Blane

#include "Platform/Platform.hpp"
#include "FileSys.hpp"
#include "Debugging/Assertion.hpp"

unsigned int FileModeToWin32Access(File::Mode mode)
{
	switch (mode)
	{
	case File::Mode::READ:
		return GENERIC_READ;
	case File::Mode::WRITE:
		return GENERIC_WRITE;
	case File::Mode::READ_WRITE:
		return GENERIC_READ | GENERIC_WRITE;
	default:
		return 0;
	}
}

unsigned int FileLocationToWin32FP(File::Location location)
{
	switch (location)
	{
	case File::Location::BEGIN:
		return FILE_BEGIN;
	case File::Location::CURRENT:
		return FILE_CURRENT;
	case File::Location::END:
		return FILE_END;
	default:
		return 0xffffffff;
	}
}

// TODO - There needs to be some way to pass in flags that dictate the file behavior. This is pretty simplistic
File::Result File::Open( File::Handle &fh, const tchar * const fileName, File::Mode mode )
{
	DWORD fileAccess = FileModeToWin32Access(mode);

	fh = CreateFile(
		fileName, 
		fileAccess, 
		FILE_SHARE_READ, 
		nullptr, 
		mode == Mode::READ ? OPEN_EXISTING : (DWORD)OPEN_ALWAYS,
		mode == Mode::READ ? FILE_ATTRIBUTE_READONLY : (DWORD)FILE_ATTRIBUTE_NORMAL,
		nullptr
	);

	if (fh == INVALID_HANDLE_VALUE)
	{
		//DWORD err = GetLastError();
		return Result::OPEN_FAIL;
	}

	return 	Result::SUCCESS;
}

File::Result File::Close( const File::Handle fh )
{
	if (!CloseHandle(fh))
	{
		return Result::CLOSE_FAIL;
	}

	return 	Result::SUCCESS;
}

File::Result File::Write( File::Handle fh, const void* const buffer, u32 inSize )
{
	if (buffer == nullptr || inSize == (size_t)-1)
	{
		return Result::WRITE_FAIL;
	}

	DWORD bytesWritten;
	bool result = WriteFile(fh, buffer, inSize, &bytesWritten, nullptr);
	if (result == false)
	{
		return Result::WRITE_FAIL;
	}

	return 	Result::SUCCESS;
}

File::Result File::Read( File::Handle fh,  void* const buffer, u32 inSize )
{
	if (buffer == nullptr)
	{
		return Result::READ_FAIL;
	}

	DWORD bytesRead;
	bool result = ReadFile(fh, buffer, inSize, &bytesRead, nullptr);

	if (result == false)
	{
		return Result::READ_FAIL;
	}

	return Result::SUCCESS;
}

File::Result File::Size(File::Handle fh, u32& fileSize)
{
	// Assumes file is at the beginning!
	File::Result result = File::Seek(fh, Location::END, 0);
	Assert(result == File::Result::SUCCESS);

	result = File::Tell(fh, fileSize);
	Assert(result == File::Result::SUCCESS);

	result = File::Seek(fh, Location::BEGIN, 0);
	return result;
}

File::Result File::Seek( File::Handle fh, File::Location location, i32 offset )
{
	DWORD moveMethod = FileLocationToWin32FP(location);
	DWORD result = SetFilePointer(fh, offset, nullptr, moveMethod);
	if (result == INVALID_SET_FILE_POINTER)
	{
		return Result::SEEK_FAIL;
	}

	return 	Result::SUCCESS;
}

File::Result File::Tell( File::Handle fh, u32 &offset )
{
	offset = SetFilePointer(fh, 0, nullptr, FILE_CURRENT);
	if (offset == INVALID_SET_FILE_POINTER)
	{
		return Result::TELL_FAIL;
	}

	return Result::SUCCESS;
}

File::Result File::Flush( File::Handle fh )
{
	bool result = FlushFileBuffers(fh);
	if (result == false)
	{
		return Result::FLUSH_FAIL;
	}

	return 	Result::SUCCESS;
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
