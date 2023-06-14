#pragma once
#include "Atlas.hpp"
#include <vector>

struct hb_glyph_info_t;
struct hb_glyph_position_t;
struct hb_buffer_t;
struct hb_font_t;

namespace Trex
{
	struct ShapedGlyph
	{
		float xOffset;
		float yOffset;
		float xAdvance;
		float yAdvance;

		Glyph info;
	};

	using ShapedGlyphs = std::vector<ShapedGlyph>;


	class TextShaper
	{
	public:
		TextShaper(const Atlas& atlas);
		~TextShaper();

		ShapedGlyphs ShapeAscii(const std::string& text);
		ShapedGlyphs ShapeUtf8(const std::string& text);
		ShapedGlyphs ShapeUtf32(const std::u32string& text);
		ShapedGlyphs ShapeUnicode(const std::vector<uint32_t>& codepoints);

	private:
		Glyph GetAtlasGlyph(uint32_t glyphIndex);
		ShapedGlyphs GetShapedGlyphs();
		ShapedGlyph GetShapedGlyph(const hb_glyph_info_t& glyphInfo, const hb_glyph_position_t& glyphPos);
		void ResetBuffer();

		AtlasGlyphs m_Glyphs;
		Glyph m_UnknownGlyph;
		std::shared_ptr<const Font> m_AtlasFont;

		hb_buffer_t* m_Buffer;
		hb_font_t* m_Font;
	};

}