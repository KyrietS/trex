
#include <gtest/gtest.h>
#include "Trex/TextShaper.hpp"

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
	constexpr char utf8Text[] = "Hello, \xc5\x9awiecie!";
	const Trex::ShapedGlyphs glyphs = shaper.ShapeUtf8(utf8Text);
	EXPECT_EQ(glyphs.size(), 16);
}

TEST_F(TextShaperTests, shouldShapeUtf32Text)
{
	const std::u32string utf32Text = U"Hello, Świecie!";
	const Trex::ShapedGlyphs glyphs = shaper.ShapeUtf32(utf32Text);
	EXPECT_EQ(glyphs.size(), utf32Text.size());
}

TEST_F(TextShaperTests, shouldShapeUnicodeText)
{
	constexpr uint32_t unicodeText[] = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 0x15a, 'w', 'i', 'e', 'c', 'i', 'e', '!'};
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
	const std::string asciiText = "Hello, World!";
	const Trex::ShapedGlyphs glyphs = shaper.ShapeAscii(asciiText);

	const Trex::TextMeasurement measurement = Trex::TextShaper::Measure(glyphs);
	EXPECT_NEAR(measurement.width, 174.5, 1.0);
	EXPECT_FLOAT_EQ(measurement.height, 29.0f);
	EXPECT_FLOAT_EQ(measurement.xOffset, 2.0f);
	EXPECT_FLOAT_EQ(measurement.yOffset, -24.0f);
	EXPECT_NEAR(measurement.xAdvance, 178.5, 1.0);
	EXPECT_FLOAT_EQ(measurement.yAdvance, 0.0f);
}
