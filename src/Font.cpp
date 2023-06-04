#include "Font.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_LCD_FILTER_H

#include <iostream>

namespace Trex
{

	FT_Library library;

	FT_Library GetFTLibrary()
	{
		if (library == nullptr)
		{
			if(FT_Init_FreeType(&library))
			{
				throw std::runtime_error("Error: could not initialize FreeType library");
			}
		}
		return library;
	}

	Font::Font(const char* path)
	{
		FT_Long faceIndex = 0; // Take the first face in the font file
		FT_Library library = GetFTLibrary();

		if(FT_New_Face(library, path, faceIndex, &face))
		{
			throw std::runtime_error("Error: could not load font");
		}

		SetSize(Points{ 12 }); // Default size
	}

	Font::Font(const Font& font)
	{
		FT_Reference_Face(font.face);
		face = font.face;
	}

	Font::Font(Font&& other) noexcept
	{
		face = other.face;
		other.face = nullptr;
	}

	Font::~Font()
	{
		FT_Done_Face(face);
	}

	void Font::SetSize(Pixels size)
	{
		FT_Error error = FT_Set_Pixel_Sizes(face, 0, size.value);
		if (error)
		{
			throw std::runtime_error("Error: could not set font size");
		}
	}

	void Font::SetSize(Points size)
	{
		FT_Error error = FT_Set_Char_Size(face, 0, size.value * 64, 0, 0); // 72 dpi used as default
		if (error)
		{
			throw std::runtime_error("Error: could not set font size");
		}
	}

	uint32_t Font::GetGlyphIndex(uint32_t codepoint) const
	{
		return FT_Get_Char_Index(face, codepoint);
	}

	Font& Font::operator=(const Font& font) noexcept
	{
		FT_Reference_Face(font.face);
		face = font.face;
		return *this;
	}

	Font& Font::operator=(Font&& other) noexcept
	{
		face = other.face;
		other.face = nullptr;

		return *this;
	}
}