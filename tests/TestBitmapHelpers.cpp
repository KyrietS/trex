#include "Trex/BitmapHelpers.hpp"
#include <gtest/gtest.h>
#include <vector>

struct BitmapHelpersTests : public ::testing::Test
{
	const std::vector<uint8_t> bitmap = {0, 127,
										 128, 255};
};

TEST_F(BitmapHelpersTests, ConvertBitmapToGrayAlpha)
{
	const std::vector<uint8_t> expected = {0, 255, 127, 255,
										   128, 255, 255, 255};
	const auto actual = Trex::ConvertBitmapToGrayAlpha(bitmap);
	EXPECT_EQ(expected, actual);
}

TEST_F(BitmapHelpersTests, ConvertBitmapToRGB)
{
	const std::vector<uint8_t> expected = {0, 0, 0, 127, 127, 127,
										   128, 128, 128, 255, 255, 255};
	const auto actual = Trex::ConvertBitmapToRGB(bitmap);
	EXPECT_EQ(expected, actual);
}

TEST_F(BitmapHelpersTests, ConvertBitmapToRGBA)
{
	const std::vector<uint8_t> expected = {0, 0, 0, 255, 127, 127, 127, 255,
										   128, 128, 128, 255, 255, 255, 255, 255};
	const auto actual = Trex::ConvertBitmapToRGBA(bitmap);
	EXPECT_EQ(expected, actual);
}
