#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>
#include "Font.hpp"

struct FT_FaceRec_;

namespace Trex
{
	class Charset
	{
	public:
		explicit Charset(std::string codepoints);
		explicit Charset(uint32_t last);
		explicit Charset(uint32_t first, uint32_t last);
		explicit Charset(std::vector<uint32_t> codepoints);

		Charset(Charset&&) = default;
		Charset(const Charset&) = default;
		Charset& operator=(const Charset&) = default;

		static Charset Full() { return Charset(); };
		static Charset Ascii() { return Charset( 0, 127 ); };

		size_t Size() const { return m_Charset.size(); }
		const std::vector<uint32_t>& Codepoints() const { return m_Charset; }
		bool IsFull() const { return m_AllCodepoints; }

	private:
		Charset() : m_Charset({ 0 }), m_AllCodepoints(true) {}

		std::vector<uint32_t> m_Charset = {};
		bool m_AllCodepoints = false;
	};

	struct Glyph
	{
		int codepoint; // Unicode codepoint
		int glyphIndex; // Index of the glyph in the font
		int x, y; // Top left corner of the glyph in the atlas
		int width, height;
		int bearingX, bearingY; 
	};

	enum class RenderMode { DEFAULT, SDF, /* LCD */ };

	using AtlasBitmap = std::vector<uint8_t>;
	using AtlasGlyphs = std::map<uint32_t, Glyph>;

	class Atlas
	{
	public:
		Atlas(const std::string& fontPath, int fontSize, const Charset&, RenderMode = RenderMode::DEFAULT, int padding = 1);
		Atlas(std::span<const uint8_t> fontData, int fontSize, const Charset&, RenderMode = RenderMode::DEFAULT, int padding = 1);

		void SetUnknownGlyph(uint32_t codepoint);
		const Glyph& GetUnknownGlyph() const;

		void SaveToFile(const std::string& path) const;
		const Glyph& GetGlyphByCodepoint(uint32_t codepoint) const;
		const Glyph& GetGlyphByIndex(uint32_t index) const;
		const AtlasBitmap& GetBitmap() const { return m_Data; }
		AtlasBitmap& GetBitmap() { return m_Data; }
		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		void UnloadBitmap() { m_Data.clear(); }

		std::shared_ptr<const Font> GetFont() const { return m_Font; }
		const AtlasGlyphs& GetGlyphs() const { return m_Glyphs; }

	private:
		void InitializeAtlas(const Charset&, RenderMode, int padding);
		void GenerateAtlas(const Charset& charset, unsigned int atlasSize, int padding, RenderMode);
		void InitializeDefaultGlyphIndex();
		void SetUnknownGlyphIndex(uint32_t index);

		std::shared_ptr<Font> m_Font;
		AtlasBitmap m_Data;
		int m_Width;
		int m_Height;
		AtlasGlyphs m_Glyphs;
		uint32_t m_UnknownGlyphIndex = 0;
	};
}