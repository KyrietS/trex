
#include <gtest/gtest.h>
#include <fstream>
#include "Trex/Atlas.hpp"

using namespace testing;
constexpr std::string_view fontPath = "fonts/Roboto-Regular.ttf";

TEST(AtlasConstructionTests, atlasShouldBeConstructibleFromFontPath)
{
	const char *pathToFont = fontPath.data();
	constexpr int fontSizeInPixels = 32;
	Trex::Atlas atlas(pathToFont, fontSizeInPixels);
}

TEST(AtlasConstructionTests, atlasShouldBeConstructibleFromFontData)
{
	std::ifstream file(fontPath.data(), std::ios::binary);
	const std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	constexpr int fontSizeInPixels = 32;
	Trex::Atlas atlas(data, fontSizeInPixels);
}

TEST(AtlasConstructionTests, shouldThrowWhenFontPathIsInvalid)
{
	const char *pathToFont = "invalid/path";
	constexpr int fontSizeInPixels = 32;
	EXPECT_THROW(Trex::Atlas atlas(pathToFont, fontSizeInPixels), std::runtime_error);
}

TEST(AtlasConstructionTests, shouldThrowWhenFontDataIsInvalid)
{
	const std::vector<uint8_t> data = { 0x00, 0x01, 0x02, 0x03 };
	constexpr int fontSizeInPixels = 32;
	EXPECT_THROW(Trex::Atlas atlas(data, fontSizeInPixels), std::runtime_error);
}

TEST(AtlasConstructionTests, shouldBeAbleToSetCharset)
{
	const Trex::Charset charset = Trex::Charset::Ascii();
	Trex::Atlas atlas(fontPath.data(), 32, charset);
}

TEST(AtlasConstructionTests, shouldBeAbleToSetRenderMode)
{
	constexpr auto renderMode = Trex::RenderMode::SDF;
	Trex::Atlas atlas(fontPath.data(), 32, Trex::Charset::Full(), renderMode);
}

TEST(AtlasConstructionTests, shouldBeAbleToSetPadding)
{
	constexpr int padding = 2;
	Trex::Atlas atlas(fontPath.data(), 32, Trex::Charset::Full(), Trex::RenderMode::DEFAULT, padding);
}

struct AtlasTests : Test
{
	Trex::Atlas atlas{ fontPath.data(), 32 };
};

TEST_F(AtlasTests, shouldSetUnknownGlyph)
{
	constexpr uint32_t codepointOfUnknownGlyph = 97;
	atlas.SetUnknownGlyph(codepointOfUnknownGlyph);
	EXPECT_EQ(atlas.GetUnknownGlyph().codepoint, codepointOfUnknownGlyph);
}

TEST_F(AtlasTests, shouldNotSetUnknownGlyphWhenCodepointIsNotInCharset)
{
	constexpr uint32_t codepointOutOfCharset = 0x123456;
	atlas.SetUnknownGlyph(codepointOutOfCharset);
	EXPECT_NE(atlas.GetUnknownGlyph().codepoint, codepointOutOfCharset);
}

TEST_F(AtlasTests, shouldGetGlyphByCodepoint)
{
	constexpr uint32_t codepoint = 'A';
	const Trex::Glyph& glyph = atlas.GetGlyphByCodepoint(codepoint);
	EXPECT_EQ(glyph.codepoint, codepoint);
}

TEST_F(AtlasTests, shouldGetInvalidGlyphWhenCodepointIsNotInCharset)
{
	constexpr uint32_t codepointOutOfCharset = 0x123456;
	const Trex::Glyph& glyph = atlas.GetGlyphByCodepoint(codepointOutOfCharset);
	EXPECT_EQ(glyph.codepoint, 0xFFFF);
}

TEST_F(AtlasTests, shouldGetGlyphByIndex)
{
	constexpr uint32_t index = 37;
	const Trex::Glyph& glyph = atlas.GetGlyphByIndex(index);
	EXPECT_EQ(glyph.glyphIndex, index);
}

TEST_F(AtlasTests, shouldGetUnknownGlyphWhenIndexIsNotInCharset)
{
	constexpr uint32_t indexOutOfCharset = 0x123456;
	const Trex::Glyph& glyph = atlas.GetGlyphByIndex(indexOutOfCharset);
	EXPECT_EQ(glyph.codepoint, atlas.GetUnknownGlyph().codepoint);
}

TEST_F(AtlasTests, shouldGetBitmap)
{
	const Trex::AtlasBitmap& bitmap = atlas.GetBitmap();
	EXPECT_FALSE(bitmap.empty());
}

TEST_F(AtlasTests, shouldGetBitmapWidth)
{
	const unsigned int width = atlas.GetWidth();
	EXPECT_EQ(width, 1024);
}

TEST_F(AtlasTests, shouldGetBitmapHeight)
{
	const unsigned int height = atlas.GetHeight();
	EXPECT_EQ(height, 1024);
}

TEST_F(AtlasTests, shouldGetFont)
{
	const auto font = atlas.GetFont();
	ASSERT_NE(font, nullptr);
	EXPECT_NE(font->face, nullptr);
}

TEST_F(AtlasTests, shouldGetGlyphs)
{
	const Trex::AtlasGlyphs& glyphs = atlas.GetGlyphs();
	EXPECT_EQ(glyphs.size(), 895);
}
