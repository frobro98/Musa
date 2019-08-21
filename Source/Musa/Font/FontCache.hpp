#pragma once

#include "File/Path.hpp"
#include "String/StringView.hpp"
#include "Containers/Map.h"

// Font Definitions

struct FontID
{
	FontID(const StringView& fontName);

	const uint32 id;
};

struct FontCharDescription
{
	uint32 uStartTexel;
	uint32 vStartTexel;
	uint32 uTexelWidth;
	uint32 vTexelHeight;
	tchar characterCode;
};

struct Font
{
	Font(StringView fontName, Texture& fontTex);
	~Font();

	Map<tchar, FontCharDescription> fontCharacterMap;
	Texture& fontTexture;
	String fontName;
	FontID id;
};

// Import ttf
FontID ImportTTFont(const Path& path);

// Get Cached Font
Font& GetCachedFont(FontID id);

