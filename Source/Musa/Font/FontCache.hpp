#pragma once

#include "File/Path.hpp"
#include "String/StringView.hpp"
#include "Containers/Map.h"

struct Texture;

// Font Definitions

struct FontID
{
	FontID(const StringView& fontName);

	friend bool operator==(const FontID& id0, const FontID& id1)
	{
		return id0.id == id1.id;
	}

	const uint32 id;
};

inline uint32 GetHash(FontID id)
{
	return id.id;
}

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
Font* GetLoadedFont(FontID id);

