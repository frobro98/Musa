#pragma once

#include "String/String.h"

class Path
{
public:
	Path() = default;
	Path(const Path&) = default;
	Path(Path&&) = default;
	Path& operator=(const Path&) = default;
	Path& operator=(Path&&) = default;

	Path(const tchar* strPath);
	Path(String&& strPath);

	bool DoesFileExist() const;
	bool DoesDirectoryExist() const;
	String GetFileExtension() const;
	String GetFileName() const;
	String GetFileNameWithoutExtension() const;
	void Normalize();
	Path GetNormalized() const;
	void MakeAbsolute();
	Path GetAbsolute() const;
	Array<String> Split() const;
	const tchar* GetString() const;

	// Operators
	Path& operator/=(const Path& otherPath);
	Path& operator/=(const String& str);
	Path& operator/=(const tchar* str);
	
private:
	void Append(const tchar* path);
	bool IsSlash(tchar c) const;
	bool HasEndingSlash(const String& pathStr) const;

	friend Path operator/(const Path& lhPath, const Path& rhPath);
	friend Path operator/(const Path& path, const String& str);
	friend Path operator/(const Path& path, const String& str);
	friend Path operator/(const String& str, const Path& path);
	friend Path operator/(const String& str, const Path& path);
	friend Path operator/(const Path& path, const tchar* str);
	friend Path operator/(const Path& path, const tchar* str);
	friend Path operator/(const tchar* str, const Path& path);
	friend Path operator/(const tchar* str, const Path& path);

private:
	String path;
};