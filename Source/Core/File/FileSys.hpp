// Copyright 2020, Nathan Blane

#pragma once

#include "Platform/Platform.hpp"
#include "Path/Path.hpp"
#include "CoreAPI.hpp"

enum class FileMode : u32
{
	Read,
	Write,
	ReadWrite
};

enum class FileLocation : u32
{
	Begin,
	Current,
	End
};

enum class FileResult : u32
{
	Success,
	OpenFail,
	CloseFail,
	WriteFail,
	ReadFail,
	SeekFail,
	TellFail,
	FlushFail
};

class CORE_API File
{
public:

	typedef void* Handle;

public:
   static FileResult Open( File::Handle &fh, const tchar * const fileName, FileMode mode );
   static FileResult Close( File::Handle fh );
   static FileResult Write( File::Handle fh, const void * const buffer, u32 inSize );
   static FileResult Read( File::Handle fh, void * const _buffer, u32 _size );
   static FileResult Size(File::Handle fh, u32& fileSize);
   static FileResult Seek( File::Handle fh, FileLocation location, i32 offset );
   static FileResult Tell( File::Handle fh, u32 &offset );
   static FileResult Flush( File::Handle fh );

   static bool DoesDirectoryExist(const Path& path);
   static bool DoesFileExist(const Path& path);
};
