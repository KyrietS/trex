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
		TextShaper(const Trex::Atlas& atlas);
		TextShaper(const TextShaper& other) : TextShaper(other.m_Atlas) {}
		~TextShaper();

		ShapedGlyphs ShapeAscii(const std::string& text);
		ShapedGlyphs ShapeUnicode(const std::vector<uint32_t>& codepoints);
		ShapedGlyphs ShapeUtf8(const std::string& text);

	private:
		ShapedGlyphs GetShapedGlyphs();
		ShapedGlyph GetShapedGlyph(const hb_glyph_info_t& glyphInfo, const hb_glyph_position_t& glyphPos);
		void ResetBuffer();

		Trex::Atlas m_Atlas;

		hb_buffer_t* m_Buffer;
		hb_font_t* m_Font;
	};

}