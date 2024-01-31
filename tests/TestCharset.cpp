#include <gtest/gtest.h>
#include "Trex/Atlas.hpp"


TEST(CharsetConstructionTests, charsetShouldBeConstructibleFromRange)
{
	Trex::Charset charset1(0, 255);
	EXPECT_EQ(charset1.Size(), 256);

	Trex::Charset charset2(0, 1'000'000);
	EXPECT_EQ(charset2.Size(), 1'000'001);
}

TEST(CharsetConstructionTests, charsetShouldBeConstructibleFromArrayOfRanges)
{
	const std::vector<Trex::Charset::Range> vec = { {0, 255}, {256, 512} };
	const Trex::Charset charset1(vec);
	EXPECT_EQ(charset1.Size(), 513);

	constexpr Trex::Charset::Range arr[] = { {0, 255}, {256, 512} };
	const Trex::Charset charset2(arr);
	EXPECT_EQ(charset2.Size(), 513);

	const Trex::Charset charset3({ {0, 255}, {256, 512} });
	EXPECT_EQ(charset3.Size(), 513);
}

TEST(CharsetConstructionTests, shouldThrowWhenRangeIsInvalid)
{
	EXPECT_THROW(Trex::Charset charset(255, 0), std::runtime_error);
}

TEST(CharsetConstructionTests, charsetShouldBeMovable)
{
	Trex::Charset charset1(0, 255);
	Trex::Charset charset2(std::move(charset1));
}

TEST(CharsetConstructionTests, charsetShouldBeCopyable)
{
	const Trex::Charset charset1(0, 255);
	Trex::Charset charset2(charset1); // NOLINT(*-unnecessary-copy-initialization)
}

TEST(CharsetConstructionTests, charsetShouldBeAssignable)
{
	const Trex::Charset charset1(0, 255);
	Trex::Charset charset2(0, 255);
	charset2 = charset1;
}

TEST(CharsetConstructionTests, asciiCharsetShouldBeConstructible)
{
	const Trex::Charset charset = Trex::Charset::Ascii();
	EXPECT_EQ(charset.Size(), 128);
	EXPECT_FALSE(charset.IsFull());
}

TEST(CharsetConstructionTests, fullCharsetShouldBeConstructible)
{
	const Trex::Charset charset = Trex::Charset::Full();
	EXPECT_TRUE(charset.IsFull());
	// Size of the full charset is undefined
	// It will be determined after loading the font
	EXPECT_NO_THROW(charset.Size());
}

TEST(CharsetConstructionTests, shouldBeAbleToAddCodepoints)
{
	Trex::Charset charset{};
	EXPECT_EQ(charset.Size(), 0);
	charset.AddCodepoint('A');
	charset.AddCodepoint(0x61);
	EXPECT_EQ(charset.Size(), 2);
}

TEST(CharsetConstructionTests, shouldNotBeAbleToAddDuplicatedCodepoints)
{
	Trex::Charset charset{};
	EXPECT_EQ(charset.Size(), 0);
	charset.AddCodepoint('A');
	charset.AddCodepoint('A');
	EXPECT_EQ(charset.Size(), 1);
}

struct CharsetTests : testing::Test
{
};

TEST_F(CharsetTests, shouldBeIterableOverCodepoints)
{
	const Trex::Charset charset = Trex::Charset::Ascii();
	std::set<uint32_t> codepoints;
	for (const auto codepoint : charset)
	{
		codepoints.insert(codepoint);
	}
	EXPECT_EQ(codepoints, charset.Codepoints());
}