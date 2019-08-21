
#include "FontCache.hpp"


#include "freetype/ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "Texture/Texture2D/TextureManager.h"
#include "Texture/Texture2D/Texture.h"

FontID ImportTTFont(const Path& path)
{
	// Ascii only
	const uint32 charCount = 256;

	FT_Library lib;
	FT_Error error = FT_Init_FreeType(&lib);
	Assert(error == FT_Err_Ok);

	FT_Face face;
	error = FT_New_Face(lib, path.GetString(), 0, &face);
	Assert(error != FT_Err_Unknown_File_Format);

	// Available glyphs in font face
	face->num_glyphs;
	// Describes face properties
	face->face_flags;
	// Only valid for scalable formats
	face->units_per_EM;
	// Number of embedded bitmap strikes (glyph images for given char pixel size)
	face->num_fixed_sizes;
	// Pointer containing pixel width and height for all strikes
	face->available_sizes;

	constexpr uint32 bitmapWidth = 1024;
	constexpr uint32 bitmapHeight = 1024;
	constexpr uint32 bitmapChannels = 4;
	constexpr uint32 sdfScaleFactor = 9;

	DynamicArray<uint8> bitmap(bitmapWidth*bitmapHeight*bitmapChannels);
	const uint32 sdfHeight = 16 * sdfScaleFactor; // size 16 font with sdf scale
	error = FT_Set_Char_Size(face, 0, sdfHeight << 6, 96, 96); // Everything it seems to be in 1/64 units, so we are shifting right to mult by 64

	Texture* t = new Texture;
	t->name = "Ariel-a";
	t->format = ImageFormat::RGBA_8norm;
	for (uint32 i = 0; i < charCount; ++i)
	{
		uint32 glyphIndex = FT_Get_Char_Index(face, 'a');
		if (glyphIndex != 0)
		{
			FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);

			FT_GlyphSlot glyph = face->glyph;
			FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);

			// Transfer bitmap information to font cache

			uint32 glyphWidth = glyph->advance.x >> 6; // Why do we shift left 6?? Everything it seems to be in 1/64 units, so we are dividing by 64
			glyphWidth *= sdfScaleFactor;
			uint32 glyphHeight = glyph->bitmap.rows;
			glyphHeight *= sdfScaleFactor;

			DynamicArray<uint8> texData(glyph->bitmap.width * glyph->bitmap.rows * 4);
			uint32 bitmapY = glyph->bitmap.rows - 1;
			for (uint32 y = 0; y < glyph->bitmap.rows; ++y)
			{
				for (uint32 x = 0; x < glyph->bitmap.width; ++x)
				{
					const uint8 bitmapVal = glyph->bitmap.buffer[x + bitmapY * glyph->bitmap.width];

					texData[(x + y * glyph->bitmap.width) * 4 + 0] = bitmapVal; // red
					texData[(x + y * glyph->bitmap.width) * 4 + 1] = bitmapVal; // green
					texData[(x + y * glyph->bitmap.width) * 4 + 2] = bitmapVal; // blue
					texData[(x + y * glyph->bitmap.width) * 4 + 3] = 255; // Alpha; TODO - put sdf information in this...
				}
				--bitmapY;
			}

			MipmapLevel level;
			level.width = glyph->bitmap.width;
			level.height = glyph->bitmap.rows;
			level.mipData = { texData.GetData(), texData.SizeInBytes() };
			t->mipLevels.Add(std::move(level));

			break;
		}
	}

	GetTextureManager().AddTexture(*t);

	error = FT_Done_FreeType(lib);

	return 0;
}