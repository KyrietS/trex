
#include <gtest/gtest.h>
#include "TextShaper.hpp"

using namespace testing;
constexpr std::string_view fontPath = "fonts/Roboto-Regular.ttf";

struct TextShaperTests : Test
{
	const Trex::Atlas atlas = Trex::Atlas(fontPath.data(), 32);
	Trex::TextShaper shaper{ atlas };
};

TEST_F(TextShaperTests, shouldShapeAsciiText)
{
	const std::string asciiText = "Hello, World!";
	const Trex::ShapedGlyphs glyphs = shaper.ShapeAscii(asciiText);
	EXPECT_EQ(glyphs.size(), asciiText.size());
}

TEST_F(TextShaperTests, shouldShapeUtf8Text)
{
	constexpr char utf8Text[] = u8"Hello, 世界!";
	const Trex::ShapedGlyphs glyphs = shaper.ShapeUtf8(utf8Text);
	EXPECT_EQ(glyphs.size(), 15);
}

TEST_F(TextShaperTests, shouldShapeUtf32Text)
{
	const std::u32string utf32Text = U"Hello, 世界!";
	const Trex::ShapedGlyphs glyphs = shaper.ShapeUtf32(utf32Text);
	EXPECT_EQ(glyphs.size(), utf32Text.size());
}

TEST_F(TextShaperTests, shouldShapeUnicodeText)
{
	constexpr uint32_t unicodeText[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 0x4e16, 0x754c, '!' };
	const Trex::ShapedGlyphs glyphs = shaper.ShapeUnicode(unicodeText);
	EXPECT_EQ(glyphs.size(), std::size(unicodeText));
}

TEST_F(TextShaperTests, shouldGetFontMetrics)
{
	const Trex::FontMetrics metrics = shaper.GetFontMetrics();
	EXPECT_EQ(metrics.ascender, 30);
	EXPECT_EQ(metrics.descender, -8);
	EXPECT_EQ(metrics.height, 38);
}

TEST_F(TextShaperTests, shouldMeasureText)
{
	constexpr char utf8Text[] = u8"Hello, 世界!";
	const Trex::ShapedGlyphs glyphs = shaper.ShapeUtf8(utf8Text);
	const Trex::TextMeasurement measurement = Trex::TextShaper::Measure(glyphs);
	EXPECT_NEAR(measurement.width, 186.0, 1.0);
	EXPECT_FLOAT_EQ(measurement.height, 37.0f);
	EXPECT_FLOAT_EQ(measurement.xOffset, 2.0f);
	EXPECT_FLOAT_EQ(measurement.yOffset, -30.0f);
	EXPECT_NEAR(measurement.xAdvance, 188.0, 1.0);
	EXPECT_FLOAT_EQ(measurement.yAdvance, 0.0f);
}
