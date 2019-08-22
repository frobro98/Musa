#pragma once

#include "File/Path.hpp"
#include "String/StringView.hpp"
#include "Containers/Map.h"

struct Texture;

// Font Definitions

struct FontID
{
	FontID(const StringView& fontName);

	const uint32 id;
};

struct FontCharDescription
{
	// Used for chars like g
	uint32 characterHeightOffset;

	uint32 uTexelStart;
	uint32 vTexelStart;
	uint32 texelWidth;
	uint32 texelHeight;
	tchar characterCode;
};

struct Font
{
	Font(const String& fontName);
	~Font();

	Map<tchar, FontCharDescription> fontCharacterMap;
	Texture* fontTexture = nullptr;
	String name;
	// Height applied to the y offset of the text when encountering newlines
	uint32 newlineHeightOffset = 0;
	FontID id;
};

// Import ttf
Font* ImportTTFont(const Path& path);

// Get Cached Font
Font& GetCachedFont(FontID id);

