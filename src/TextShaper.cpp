#include "TextShaper.hpp"
#include "hb.h"
#include "hb-ft.h"

namespace Trex
{

	Trex::TextShaper::TextShaper(const Trex::Atlas& atlas)
		: m_Atlas(atlas),
		m_Buffer(hb_buffer_create()),
		m_Font(hb_ft_font_create_referenced(atlas.GetFontFace()))
	{
		m_Atlas.UnloadBitmap(); // We don't need the bitmap here
	}

	Trex::TextShaper::~TextShaper()
	{
		hb_buffer_destroy(m_Buffer);
		hb_font_destroy(m_Font);
	}

	ShapedGlyphs TextShaper::ShapeAscii(const std::string& text)
	{
		std::vector<uint32_t> codepoints(text.begin(), text.end());
		return ShapeUnicode(codepoints);
	}

	ShapedGlyphs TextShaper::ShapeUtf8(const std::string& text)
	{
		ResetBuffer();
		hb_buffer_add_utf8(m_Buffer, text.c_str(), (int)text.size(), 0, (int)text.size());
		hb_shape(m_Font, m_Buffer, nullptr, 0);

		return GetShapedGlyphs();
	}

	ShapedGlyphs TextShaper::ShapeUtf32(const std::u32string& text)
	{
		// Unfortunately casting text.c_str() to uint32_t* violates the strict aliasing rule.
		// char32_t and uint32_t are not the same type, even though they are both 32 bits wide.
		std::vector<uint32_t> codepoints(text.begin(), text.end());
		return ShapeUnicode(codepoints);
	}

	ShapedGlyphs TextShaper::ShapeUnicode(const std::vector<uint32_t>& codepoints)
	{
		ResetBuffer();
		hb_buffer_add_codepoints(m_Buffer, codepoints.data(), (int)codepoints.size(), 0, (int)codepoints.size());
		hb_shape(m_Font, m_Buffer, nullptr, 0);

		return GetShapedGlyphs();
	}

	ShapedGlyphs TextShaper::GetShapedGlyphs()
	{
		unsigned int glyphCount;
		hb_glyph_info_t* glyphInfo = hb_buffer_get_glyph_infos(m_Buffer, &glyphCount);
		hb_glyph_position_t* glyphPos = hb_buffer_get_glyph_positions(m_Buffer, &glyphCount);

		ShapedGlyphs glyphs;
		glyphs.reserve(glyphCount);
		for (unsigned int i = 0; i < glyphCount; i++)
		{
			ShapedGlyph glyph = GetShapedGlyph(glyphInfo[i], glyphPos[i]);
			glyphs.push_back(glyph);
		}

		return glyphs;
	}

	ShapedGlyph TextShaper::GetShapedGlyph(const hb_glyph_info_t& glyphInfo, const hb_glyph_position_t& glyphPos)
	{
		unsigned int glyphIndex = glyphInfo.codepoint; // after shaping codepoint becomes glyph index
		ShapedGlyph glyph;
		glyph.info = m_Atlas.GetGlyphByIndex(glyphIndex);
		glyph.xOffset = glyphPos.x_offset / 64.0f;
		glyph.yOffset = glyphPos.y_offset / 64.0f;
		glyph.xAdvance = glyphPos.x_advance / 64.0f;
		glyph.yAdvance = glyphPos.y_advance / 64.0f;
		return glyph;
	}

	void TextShaper::ResetBuffer()
	{
		hb_buffer_reset(m_Buffer);
		hb_buffer_set_direction(m_Buffer, HB_DIRECTION_LTR);
		hb_buffer_set_script(m_Buffer, HB_SCRIPT_COMMON);
		hb_buffer_set_language(m_Buffer, hb_language_from_string("pl", -1));
	}
}