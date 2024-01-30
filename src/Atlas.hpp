#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <span>
#include "Font.hpp"
#include "Charset.hpp"

struct FT_FaceRec_;

namespace Trex
{
	struct Glyph
	{
		unsigned int codepoint; // Unicode codepoint
		unsigned int glyphIndex; // Index of the glyph in the font
		int x, y; // Top left corner of the glyph in the atlas
		unsigned int width, height;
		int bearingX, bearingY; 
	};

	enum class RenderMode { DEFAULT, SDF, /* LCD */ };

	using AtlasBitmap = std::vector<uint8_t>;
	using AtlasGlyphs = std::map<uint32_t, Glyph>;

	class Atlas
	{
	public:
		Atlas(const std::string& fontPath, int fontSize, const Charset& = Charset::Full(), RenderMode = RenderMode::DEFAULT, int padding = 1);
		Atlas(std::span<const uint8_t> fontData, int fontSize, const Charset& = Charset::Full(), RenderMode = RenderMode::DEFAULT, int padding = 1);

		void SetUnknownGlyph(uint32_t codepoint);
		const Glyph& GetUnknownGlyph() const;

		void SaveToFile(const std::string& path) const;
		const Glyph& GetGlyphByCodepoint(uint32_t codepoint) const;
		const Glyph& GetGlyphByIndex(uint32_t index) const;
		const AtlasBitmap& GetBitmap() const { return m_Data; }
		AtlasBitmap& GetBitmap() { return m_Data; }
		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::shared_ptr<const Font> GetFont() const { return m_Font; }
		const AtlasGlyphs& GetGlyphs() const { return m_Glyphs; }

	private:
		void InitializeAtlas(const Charset&, RenderMode, int padding);
		void GenerateAtlas(const Charset& charset, unsigned int atlasSize, int padding, RenderMode);
		void InitializeDefaultGlyphIndex();
		void SetUnknownGlyphIndex(uint32_t index);

		std::shared_ptr<Font> m_Font;
		AtlasBitmap m_Data;
		unsigned int m_Width{};
		unsigned int m_Height{};
		AtlasGlyphs m_Glyphs;
		uint32_t m_UnknownGlyphIndex = 0;
	};
}