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

	enum class RenderMode { DEFAULT, SDF, LCD };
	enum class PixelFormat { GRAY, RGB, BGRA };

	using AtlasGlyphs = std::map<uint32_t, Glyph>; // key: glyph index in font (NOT codepoint!)

	class Atlas
	{
	public:
		Atlas(const std::string& fontPath, int fontSize, const Charset& = Charset::Full(), RenderMode = RenderMode::DEFAULT, int padding = 1);
		Atlas(std::span<const uint8_t> fontData, int fontSize, const Charset& = Charset::Full(), RenderMode = RenderMode::DEFAULT, int padding = 1);

		struct FreeTypeGlyph;
		class Bitmap;

		void SetUnknownGlyph(uint32_t codepoint); // glyphs
		const Glyph& GetUnknownGlyph() const; // glyphs

		void SaveToFile(const std::string& path) const;
		const Glyph& GetGlyphByCodepoint(uint32_t codepoint) const; // glyphs
		const Glyph& GetGlyphByIndex(uint32_t index) const; // glyphs
		const Bitmap& GetBitmap() const { return m_Bitmap; }

		std::shared_ptr<const Font> GetFont() const { return m_Font; }
		const AtlasGlyphs& GetGlyphs() const { return m_Glyphs; }

		class Bitmap
		{
		public:
			Bitmap() = default;
			Bitmap(unsigned int width, unsigned int height, PixelFormat format);

			const std::vector<uint8_t>& Data() const { return m_Data; }
			unsigned int Width() const { return m_Width; }
			unsigned int Height() const { return m_Height; }
			PixelFormat Format() const { return m_Format; }
			unsigned int Channels() const;

			void Draw(int x, int y, const FreeTypeGlyph&);
		private:
			std::vector<uint8_t> m_Data {};
			unsigned int m_Width {};
			unsigned int m_Height {};
			PixelFormat m_Format {};
		};

	private:
		void InitializeAtlas(const Charset&, RenderMode, int padding);
		void InitializeDefaultGlyphIndex();
		void SetUnknownGlyphIndex(uint32_t index);

		std::shared_ptr<Font> m_Font;
		Bitmap m_Bitmap;
		AtlasGlyphs m_Glyphs;
		uint32_t m_UnknownGlyphIndex = 0;
	};
}