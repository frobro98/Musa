#pragma once

// Make the assumption of c-char strings, not UNICODE
// 32 bit files, not supporting 64 bits

#include "Platform.h"

// TODO - Refactor the shit out of this.........
class File
{
public:

	typedef void *Handle;

	enum class Mode : uint32
	{
		READ,
		WRITE,
		READ_WRITE
	};

	enum class Location : uint32
	{
		BEGIN,
		CURRENT,
		END
	};

	enum class Result : uint32
	{
		SUCCESS,
		OPEN_FAIL,
		CLOSE_FAIL,
		WRITE_FAIL,
		READ_FAIL,
		SEEK_FAIL,
		TELL_FAIL,
		FLUSH_FAIL
	};

public:
   static File::Result Open( File::Handle &fh, const tchar * const fileName, File::Mode mode );
   static File::Result Close( File::Handle fh );
   static File::Result Write( File::Handle fh, const void * const buffer, uint32 inSize );
   static File::Result Read( File::Handle fh, void * const _buffer, uint32 _size );
   static File::Result Size(File::Handle fh, uint32& fileSize);
   static File::Result Seek( File::Handle fh, File::Location location, int32 offset );
   static File::Result Tell( File::Handle fh, uint32 &offset );
   static File::Result Flush( File::Handle fh );

};
