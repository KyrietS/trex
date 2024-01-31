
#include <gtest/gtest.h>
#include <fstream>
#include "Font.hpp"

using namespace testing;
constexpr std::string_view fontPath = "fonts/Roboto-Regular.ttf";


TEST(FontConstructionTests, fontShouldBeConstructibleFromPath)
{
	const char *path = fontPath.data();
	Trex::Font font(path);
}

TEST(FontConstructionTests, fontShouldBeConstructibleFromArrayOfUint8)
{
	std::ifstream file(fontPath.data(), std::ios::binary);
	const std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	Trex::Font font(data);
}

TEST(FontConstructionTests, fontShouldBeConstructibleFromArrayOfChars)
{
	std::ifstream file(fontPath.data(), std::ios::binary);
	const std::vector<char> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	Trex::Font font(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(data.data()), data.size()));
}

TEST(FontConstructionTests, shouldThrowWhenFontPathIsInvalid)
{
	const char *path = "invalid/path";
	EXPECT_THROW(Trex::Font font(path), std::runtime_error);
}

TEST(FontConstructionTests, shouldThrowWhenFontDataIsInvalid)
{
	const std::vector<uint8_t> data = { 0x00, 0x01, 0x02, 0x03 };
	EXPECT_THROW(Trex::Font font(data), std::runtime_error);
}

TEST(FontConstructionTests, fontShouldBeMovable)
{
	const char *path = fontPath.data();
	Trex::Font font1(path);
	Trex::Font font2(std::move(font1));
}

struct FontTests : Test
{
	const char *path = fontPath.data();
	Trex::Font font{ path };
};

TEST_F(FontTests, faceShouldNotBeNullptr)
{
	EXPECT_NE(font.face, nullptr);
}

TEST_F(FontTests, shouldChangeSizeInPixels)
{
	font.SetSize(Trex::Pixels{ 12 });
}

TEST_F(FontTests, shouldChangeSizeInPoints)
{
	font.SetSize(Trex::Points{ 12 });
}

TEST_F(FontTests, shouldGetGlyphIndex)
{
	constexpr uint32_t codepointA = 'A';
	constexpr uint32_t codepointB = 'B';
	EXPECT_EQ(font.GetGlyphIndex(codepointA), 37);
	EXPECT_EQ(font.GetGlyphIndex(codepointB), 38);
}

TEST_F(FontTests, shouldGetGlyphIndexZeroForSpecialCharacters)
{
	constexpr uint32_t codepointTab = '\t';
	constexpr uint32_t codepointNewline = '\n';
	EXPECT_EQ(font.GetGlyphIndex(codepointTab), 0);
	EXPECT_EQ(font.GetGlyphIndex(codepointNewline), 0);
}

TEST_F(FontTests, shouldGetGlyphIndexZeroForInvalidCodepoint)
{
	constexpr uint32_t codepoint = 0xFFFFFFFF;
	EXPECT_EQ(font.GetGlyphIndex(codepoint), 0);
}

TEST_F(FontTests, shouldGetValidFontMetricsForSize12)
{
	font.SetSize(Trex::Pixels{ 12 });
	const Trex::FontMetrics metrics = font.GetMetrics();
	EXPECT_EQ(metrics.ascender, 12);
	EXPECT_EQ(metrics.descender, -3);
	EXPECT_EQ(metrics.height, 14);
}

TEST_F(FontTests, shouldGetValidFontMetricsForSize24)
{
	font.SetSize(Trex::Pixels{ 24 });
	const Trex::FontMetrics metrics = font.GetMetrics();
	EXPECT_EQ(metrics.ascender, 23);
	EXPECT_EQ(metrics.descender, -6);
	EXPECT_EQ(metrics.height, 28);
}
