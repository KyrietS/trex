
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

TEST(AtlasConstructionTests, shouldBeAbleToSetSdfRenderMode)
{
	Trex::Atlas atlas(fontPath.data(), 32, Trex::Charset::Full(), Trex::RenderMode::SDF);
}

TEST(AtlasConstructionTests, shouldBeAbleToSetLcdRenderMode)
{
	Trex::Atlas atlas(fontPath.data(), 32, Trex::Charset::Full(), Trex::RenderMode::LCD);
}

TEST(AtlasConstructionTests, shouldBeAbleToSetPadding)
{
	constexpr int padding = 2;
	Trex::Atlas atlas(fontPath.data(), 32, Trex::Charset::Full(), Trex::RenderMode::DEFAULT, padding);
}

struct AtlasTests : Test
{
	AtlasTests() = default;

	Trex::Atlas atlas{ fontPath.data(), 32 };
};

TEST_F(AtlasTests, shouldGetFont)
{
	const auto font = atlas.GetFont();
	ASSERT_NE(font, nullptr);
	EXPECT_NE(font->face, nullptr);
}

struct AtlasGlyphsTests : Test
{
	AtlasGlyphsTests() : atlas{ fontPath.data(), 32 }, glyphs{atlas.GetGlyphs()} {}

	Trex::Atlas atlas;
	const Trex::Atlas::Glyphs& glyphs;
};

TEST_F(AtlasGlyphsTests, shouldContainAllGlyphs)
{
	EXPECT_FALSE(glyphs.Empty());
	EXPECT_EQ(glyphs.Data().size(), 895);
}

TEST_F(AtlasGlyphsTests, shouldSetUnknownGlyph)
{
	constexpr uint32_t codepointOfUnknownGlyph = 97;
	glyphs.SetUnknownGlyph(codepointOfUnknownGlyph);
	EXPECT_EQ(glyphs.GetUnknownGlyph().codepoint, codepointOfUnknownGlyph);
}

TEST_F(AtlasGlyphsTests, shouldNotSetUnknownGlyphWhenCodepointIsNotInCharset)
{
	constexpr uint32_t codepointOutOfCharset = 0x123456;
	glyphs.SetUnknownGlyph(codepointOutOfCharset);
	EXPECT_NE(glyphs.GetUnknownGlyph().codepoint, codepointOutOfCharset);
}

TEST_F(AtlasGlyphsTests, shouldGetGlyphByCodepoint)
{
	constexpr uint32_t codepoint = 'A';
	const Trex::Glyph& glyph = glyphs.GetGlyphByCodepoint(codepoint);
	EXPECT_EQ(glyph.codepoint, codepoint);
}

TEST_F(AtlasGlyphsTests, shouldGetInvalidGlyphWhenCodepointIsNotInCharset)
{
	constexpr uint32_t codepointOutOfCharset = 0x123456;
	const Trex::Glyph& glyph = glyphs.GetGlyphByCodepoint(codepointOutOfCharset);
	EXPECT_EQ(glyph.codepoint, 0xFFFF);
}

TEST_F(AtlasGlyphsTests, shouldGetGlyphByIndex)
{
	constexpr uint32_t index = 37;
	const Trex::Glyph& glyph = glyphs.GetGlyphByIndex(index);
	EXPECT_EQ(glyph.glyphIndex, index);
}

TEST_F(AtlasGlyphsTests, shouldGetUnknownGlyphWhenIndexIsNotInCharset)
{
	constexpr uint32_t indexOutOfCharset = 0x123456;
	const Trex::Glyph& glyph = glyphs.GetGlyphByIndex(indexOutOfCharset);
	EXPECT_EQ(glyph.codepoint, glyphs.GetUnknownGlyph().codepoint);
}

struct AtlasBitmapTests : Test
{
	AtlasBitmapTests() : atlas{ fontPath.data(), 32 }, bitmap{atlas.GetBitmap()} {}

	Trex::Atlas atlas;
	const Trex::Atlas::Bitmap& bitmap;
};

TEST_F(AtlasBitmapTests, shouldGetBitmap)
{
	EXPECT_FALSE(bitmap.Data().empty());
}

TEST_F(AtlasBitmapTests, shouldGetBitmapWidth)
{
	const unsigned int width = bitmap.Width();
	EXPECT_EQ(width, 1024);
}

TEST_F(AtlasBitmapTests, shouldGetBitmapHeight)
{
	const unsigned int height = bitmap.Height();
	EXPECT_EQ(height, 1024);
}
