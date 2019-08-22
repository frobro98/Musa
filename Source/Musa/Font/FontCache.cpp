
#include "FontCache.hpp"


#include "freetype/ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "Texture/Texture2D/TextureManager.h"
#include "Texture/Texture2D/Texture.h"
#include "Utilities/CoreUtilities.hpp"

//////////////////////////////////////////////////////////////////////////
// FontID
//////////////////////////////////////////////////////////////////////////

FontID::FontID(const StringView& fontName)
	: id(GetHash(fontName))
{
}

//////////////////////////////////////////////////////////////////////////
// FontCharDescription
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Font
//////////////////////////////////////////////////////////////////////////
Font::Font(const String& fontName)
	: name(fontName),
	id(StringView{ *fontName })
{
}

Font::~Font()
{

}

//////////////////////////////////////////////////////////////////////////
// Font Caching
//////////////////////////////////////////////////////////////////////////

static constexpr uint32 ftToPixelSpace(uint32 fontSpaceVal)
{
	return fontSpaceVal >> 6;
}

static constexpr uint32 ftToFontSpace(uint32 pixelSpaceVal)
{
	return pixelSpaceVal << 6;
}

Font* ImportTTFont(const Path& path)
{
	// Ascii only
	constexpr uint32 charCount = 256;

	FT_Library lib;
	FT_Error error = FT_Init_FreeType(&lib);
	Assert(error == FT_Err_Ok);

	FT_Face face;
	error = FT_New_Face(lib, path.GetString(), 0, &face);
	Assert(error != FT_Err_Unknown_File_Format);

	constexpr uint32 bitmapWidth = 1024;
	constexpr uint32 bitmapHeight = 1024;
	constexpr uint32 bitmapChannels = 4;
	constexpr uint32 sdfScaleFactor = 1;

	String fontName = "Ariel";
	Font* font = new Font(fontName);

	DynamicArray<uint8> bitmap(bitmapWidth*bitmapHeight*bitmapChannels);
	const uint32 sdfHeight = 48 * sdfScaleFactor; // size 16 font with sdf scale
	error = FT_Set_Char_Size(face, 0, ftToFontSpace(sdfHeight), 96, 96); // Everything it seems to be in 1/64 units, so we are shifting right to mult by 64

	uint32 acenderInPixels = ftToPixelSpace(FT_MulFix(face->ascender, face->size->metrics.y_scale));

	Texture* t = new Texture;
	t->name = fontName;
	t->format = ImageFormat::RGBA_8norm;

	uint32 bitmapWidthOffset = 0;
	uint32 bitmapHeightOffset = 0;
	uint32 maxCharHeight = 0;
	for (uint32 i = 0; i < charCount; ++i)
	{
		tchar c = (tchar)i;

		uint32 glyphIndex = FT_Get_Char_Index(face, i);
		if (glyphIndex != 0)
		{
			FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);

			FT_GlyphSlot glyph = face->glyph;
			FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);

			// Transfer bitmap information to font cache

			uint32 glyphWidth = ftToPixelSpace(glyph->advance.x); // Why do we shift left 6?? Everything it seems to be in 1/64 units, so we are dividing by 64
			glyphWidth *= sdfScaleFactor;
			uint32 glyphHeight = glyph->bitmap.rows;
			glyphHeight *= sdfScaleFactor;

			// Move to next row
			if (bitmapWidthOffset + glyphWidth > bitmapWidth)
			{
				bitmapWidthOffset = 0;
				bitmapHeightOffset += maxCharHeight;
				maxCharHeight = 0;
			}

			Assert(bitmapHeightOffset + glyphHeight < bitmapHeight);

			maxCharHeight = Max(maxCharHeight, glyphHeight);

			FontCharDescription desc = {};
			desc.characterCode = c;
			desc.uTexelStart = bitmapWidthOffset;
			desc.vTexelStart = bitmapHeightOffset;
			desc.texelWidth = bitmapWidth;
			desc.texelHeight = bitmapHeight;
			desc.characterHeightOffset = acenderInPixels - glyph->bitmap_top;

			font->fontCharacterMap.Add(c, desc);

			uint32 bitmapY = glyph->bitmap.rows - 1;
			for (uint32 y = 0; y < glyph->bitmap.rows; ++y)
			{
				for (uint32 x = 0; x < glyph->bitmap.width; ++x)
				{
					const uint8 bitmapVal = glyph->bitmap.buffer[x + bitmapY * glyph->bitmap.width];
					const uint32 xTexPos = x + bitmapWidthOffset;
					const uint32 yTexPos = y + bitmapHeightOffset;
					const uint32 totalArrayPos = (xTexPos + yTexPos * bitmapWidth) * 4;

					// TODO - Because of blending, there needs to be some meaningful alpha value. 
					// TODO - The alpha lives in the bitmap, so it doesn't really make much sense to store it multiple times. Just keep the alpha for the entire pixel, disregarding the other 3 values
					bitmap[totalArrayPos + 0] = bitmapVal; // red
					bitmap[totalArrayPos + 1] = bitmapVal; // green
					bitmap[totalArrayPos + 2] = bitmapVal; // blue
					bitmap[totalArrayPos + 3] = 255; // Alpha; TODO - put sdf information in this...
				}
				--bitmapY;
			}

			bitmapWidthOffset += glyphWidth;

		}
	}


	MipmapLevel level;
	level.width = bitmapWidth;
	level.height = bitmapHeight;
	level.mipData = { bitmap.GetData(), bitmap.SizeInBytes() };
	t->mipLevels.Add(std::move(level));

	font->fontTexture = t;

	GetTextureManager().AddTexture(*t);

	error = FT_Done_FreeType(lib);

	return font;
}

