#include "Atlas.hpp"
#include <cstdint>
#include <vector>
#include <string_view>
#include "Font.hpp"
#include <ft2build.h>
#include <stdexcept>
#include <map>
#include <optional>
#include <cassert>
#include "stb_image_write.h"
#include "../lib/freetype/src/sdf/ftsdfrend.h"
#include FT_FREETYPE_H
#include FT_RENDER_H

namespace Trex
{
	Charset::Charset(std::string codepoints)
	{
		m_Charset.reserve(codepoints.size());
		for (const char codepoint : codepoints)
		{
			m_Charset.push_back(codepoint);
		}
	}

	Charset::Charset(uint32_t last)
		: Charset(0, last)
	{
	}

	Charset::Charset(uint32_t first, uint32_t last)
	{
		assert(first <= last);
		m_Charset.reserve(last - first + 1);
		for (uint32_t codepoint = first; codepoint <= last; codepoint++)
		{
			m_Charset.push_back(codepoint);
		}
	}

	Charset::Charset(std::vector<uint32_t> codepoints)
		: m_Charset(std::move(codepoints))
	{
	}

	struct GlyphMetrics
	{
		unsigned int width;
		unsigned int height;
	};

	Glyph GetGlyphInfo(FT_GlyphSlot ft_glyph, int x, int y, uint32_t codepoint)
	{
		Glyph glyph;
		glyph.x = x;
		glyph.y = y;
		glyph.codepoint = codepoint;
		glyph.glyphIndex = ft_glyph->glyph_index;
		glyph.width = ft_glyph->bitmap.width;
		glyph.height = ft_glyph->bitmap.rows;
		glyph.bearingX = ft_glyph->metrics.horiBearingX / 64;
		glyph.bearingY = ft_glyph->metrics.horiBearingY / 64;

		return glyph;
	}

	void AdjustGlyphSizeForSdfRendering(FT_GlyphSlot& glyph)
	{
		// Get spread value from SDF renderer module
		FT_Library library = glyph->library;
		SDF_Renderer sdfModule = (SDF_Renderer)FT_Get_Module(library, "sdf");
		FT_UInt spread = sdfModule->spread;

		// Adjust glyph size. See ftsdfrend.c for more details. (function: ft_sdf_render)
		glyph->bitmap.width += spread * 2;
		glyph->bitmap.rows += spread * 2;
	}

	// Note: calling this function will invalidate the previous FT_GlyphSlot returned.
	FT_GlyphSlot LoadGlyphWithoutRender(FT_Face fontFace, uint32_t codepoint, RenderMode mode)
	{
		FT_Error error = FT_Load_Char(fontFace, codepoint, FT_LOAD_DEFAULT);
		if (error)
		{
			throw std::runtime_error("Error: could not load and render char");
		}
		if (mode == RenderMode::SDF)
		{
			AdjustGlyphSizeForSdfRendering(fontFace->glyph);
		}

		return fontFace->glyph;
	}

	FT_Render_Mode GetFreeTypeRenderMode(RenderMode mode)
	{
		switch (mode)
		{
		case RenderMode::DEFAULT:
			return FT_RENDER_MODE_NORMAL;
		case RenderMode::SDF:
			return FT_RENDER_MODE_SDF;
		default:
			throw std::runtime_error("Error: unknown render mode!");
		}
	}

	FT_GlyphSlot LoadGlyphWithRender(FT_Face fontFace, uint32_t codepoint, RenderMode mode)
	{
		auto glyph = LoadGlyphWithoutRender(fontFace, codepoint, mode);

		// Use bsdf renderer instead of sdf renderer.
		// See: https://freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_render_mode
		// First I need to render the glyph with normal mode, then render it with sdf mode.
		// But the bsdf renderer cannot handle the glyph with zero width or height (e.g. space).
		FT_Error error = FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
		bool isGlyphEmpty = glyph->bitmap.width == 0 || glyph->bitmap.rows == 0;
		if (mode == RenderMode::SDF and not isGlyphEmpty and not error)
			error = FT_Render_Glyph(glyph, FT_RENDER_MODE_SDF);

		// Use sdf renderer. This renderer uses outlines instead of bitmap.
		// The benchmark shows that this renderer is 2 times slower than bsdf renderer.
		// FT_Error error = FT_Render_Glyph(glyph, GetFreeTypeRenderMode(mode));

		if (error)
		{
			throw std::runtime_error("Error: could not load and render char");
		}
		return glyph;
	}

	std::vector<GlyphMetrics> GetGlyphsMetrics(FT_Face fontFace, const Charset& charset, RenderMode mode)
	{
		std::vector<GlyphMetrics> allMetrics;
		allMetrics.reserve(charset.Size());
		for (uint32_t codepoint : charset.Codepoints())
		{
			auto glyph = LoadGlyphWithoutRender(fontFace, codepoint, mode);

			GlyphMetrics metrics
			{
				.width = glyph->bitmap.width,
				.height = glyph->bitmap.rows
			};
			allMetrics.push_back(metrics);
		}

		return allMetrics;
	}


	bool IsAtlasSizeEnough(const std::vector<GlyphMetrics>& metrics, unsigned int atlasSize, int padding)
	{
		int x = 0;
		int y = 0;
		unsigned int maxHeight = 0;
		for (const auto& glyph : metrics)
		{
			unsigned int glyphWidth = glyph.width + padding * 2;
			unsigned int glyphHeight = glyph.height + padding * 2;

			maxHeight = std::max(maxHeight, glyphHeight);
			if (x + glyphWidth > atlasSize) // Next row
			{
				x = 0;
				y += maxHeight;
				maxHeight = 0;
			}
			if (y + glyphHeight > atlasSize)
			{
				return false;
			}
			x += glyphWidth;
		}
		return true;
	}

	unsigned int GetAtlasSize(const std::vector<GlyphMetrics>& glyphsMetrics, int padding)
	{
		unsigned int atlasSize = 128; // Start with 128x128
		while (not IsAtlasSizeEnough(glyphsMetrics, atlasSize, padding))
		{
			atlasSize *= 2;
		}

		return atlasSize;
	}

	std::pair<AtlasBitmap, AtlasGlyphs> BuildAtlasBitmap(Font& font, const Charset& charset, unsigned int atlasSize, int padding, RenderMode mode)
	{
		AtlasBitmap bitmap(atlasSize * atlasSize);
		std::fill(bitmap.begin(), bitmap.end(), 255); // Fill with white
		AtlasGlyphs glyphs;

		FT_Face face = font.face;
		unsigned int atlasX = 0;
		unsigned int atlasY = 0;
		unsigned int maxHeight = 0;
		for (uint32_t codepoint : charset.Codepoints())
		{
			auto glyph = LoadGlyphWithRender(face, codepoint, mode);
			unsigned int glyphWidth = glyph->bitmap.width;
			unsigned int glyphHeight = glyph->bitmap.rows;

			unsigned int glyphWidthPadding = glyphWidth + padding * 2;
			unsigned int glyphHeightPadding = glyphHeight + padding * 2;

			maxHeight = std::max(maxHeight, glyphHeightPadding);
			// If we are out of atlas bounds, go to the next line
			if (atlasX + glyphWidthPadding > atlasSize)
			{
				atlasX = 0;
				atlasY += maxHeight;
				maxHeight = 0;
			}
			// Copy glyph bitmap to atlas bitmap
			for (unsigned int glyphY = 0; glyphY < glyphHeight; ++glyphY)
			{
				for (unsigned int glyphX = 0; glyphX < glyphWidth; ++glyphX)
				{
					int atlasBitmapIndex = (atlasY + glyphY + padding) * atlasSize + (atlasX + glyphX + padding);
					int glyphIndex = glyphY * glyphWidth + glyphX;
					bitmap.at(atlasBitmapIndex) = 255 - glyph->bitmap.buffer[glyphIndex];
				}
			}

			int x0 = atlasX + padding;
			int y0 = atlasY + padding;
			glyphs[glyph->glyph_index] = GetGlyphInfo(glyph, x0, y0, codepoint);

			atlasX += glyphWidthPadding;
		}

		return { bitmap, glyphs };
	}

	void Atlas::GenerateAtlas(const Charset& charset, unsigned int atlasSize, int padding, RenderMode mode)
	{
		this->m_Width = atlasSize;
		this->m_Height = atlasSize;

		const auto& [bitmap, glyphs] = BuildAtlasBitmap(m_Font, charset, atlasSize, padding, mode);
		this->m_Data = bitmap;
		this->m_Glyphs = glyphs;
	}

	Charset GetFullCharsetFilled(const Charset& charset, Font& font)
	{
		std::vector<uint32_t> codepoints;
		codepoints.push_back(0xFFFF); // Add unknown glyph. It will have index 0.

		FT_UInt nextGlyphIndex;
		FT_ULong codepoint = FT_Get_First_Char(font.face, &nextGlyphIndex);

		while (nextGlyphIndex != 0)
		{
			codepoints.push_back(codepoint);
			codepoint = FT_Get_Next_Char(font.face, codepoint, &nextGlyphIndex);
		}

		return Charset{ codepoints };
	}

	Atlas::Atlas(const std::string& fontPath, int fontSize, const Charset& charset, RenderMode mode, int padding)
		: m_Font(fontPath.c_str())
	{
		this->m_Font.SetSize(Pixels{ fontSize });

		const Charset filledCharset = charset.IsFull() ? GetFullCharsetFilled(charset, m_Font) : charset;

		auto metrics = GetGlyphsMetrics(m_Font.face, filledCharset, mode);
		auto atlasSize = GetAtlasSize(metrics, padding);

		GenerateAtlas(filledCharset, atlasSize, padding, mode);
		InitializeDefaultGlyphIndex();
	}

	void Atlas::InitializeDefaultGlyphIndex()
	{
		if (m_Glyphs.size() == 0)
		{
			throw std::runtime_error("Error: cannot set default glyph in empty atlas");
		}

		SetDefaultGlyphIndex(m_Glyphs.begin()->first); // Set first glyph as default
		SetDefaultGlyphIndex(0); // Try to set 'undefined character code' as default
		SetDefaultGlyph(0xFFFD); // Try to set 'unicode replacement character' as default
	}

	void Atlas::SetDefaultGlyph(uint32_t codepoint)
	{
		auto index = m_Font.GetGlyphIndex(codepoint);
		SetDefaultGlyphIndex(index);
	}

	void Atlas::SetDefaultGlyphIndex(uint32_t index)
	{
		if (m_Glyphs.contains(index))
		{
			m_DefaultGlyphIndex = index;
		}
	}

	void Atlas::SaveToFile(const std::string& path) const
	{
		constexpr int GRAY_SCALE = 1;

		if (path.ends_with(".png"))
		{
			stbi_write_png(path.c_str(), m_Width, m_Height, GRAY_SCALE, m_Data.data(), m_Width);
		}
		else if (path.ends_with(".bmp"))
		{
			stbi_write_bmp(path.c_str(), m_Width, m_Height, GRAY_SCALE, m_Data.data());
		}
		else
		{
			throw std::runtime_error("Error: unsupported file format");
		}
	}

	const Glyph& Atlas::GetGlyphByCodepoint(uint32_t codepoint) const
	{
		return GetGlyphByIndex(m_Font.GetGlyphIndex(codepoint));
	}

	const Glyph& Atlas::GetGlyphByIndex(uint32_t index) const
	{
		return m_Glyphs.contains(index) ? m_Glyphs.at(index) : m_Glyphs.at(m_DefaultGlyphIndex);
	}

}