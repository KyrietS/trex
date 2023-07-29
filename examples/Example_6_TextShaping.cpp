#include "Atlas.hpp"
#include "TextShaper.hpp"
#include "raylib.h"

Image GetAtlasAsBitmapImage(Trex::Atlas& atlas)
{
	Image atlasImage;
	atlasImage.data = atlas.GetBitmap().data(); // pointer to the atlas bitmap data
	atlasImage.width = (int)atlas.GetWidth(); // width of the atlas bitmap
	atlasImage.height = (int)atlas.GetHeight(); // height of the atlas bitmap
	atlasImage.mipmaps = 1;
	atlasImage.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE; // atlas bitmap format is always 1 byte per pixel (grayscale)
	return atlasImage;
}

void RenderGlyph(float x, float y, const Trex::Glyph& glyph, Texture2D& atlasTexture)
{
	Rectangle atlasFragment = {
		.x = (float)glyph.x, // top-left corner of the glyph in the atlas
		.y = (float)glyph.y, // top-left corner of the glyph in the atlas
		.width = (float)glyph.width, // width of the glyph in the atlas
		.height = (float)glyph.height // height of the glyph in the atlas
	};

	// Draw a texture fragment from the atlas
	DrawTextureRec(atlasTexture, atlasFragment, { x, y }, WHITE);
}

void RenderShapedText(float cursorX, float cursorY, const Trex::ShapedGlyphs& glyphs, Texture2D& atlasTexture)
{
	for (const auto& glyph : glyphs)
	{
		float x = cursorX + glyph.xOffset + (float)glyph.info.bearingX;
		float y = cursorY + glyph.yOffset - (float)glyph.info.bearingY;

		RenderGlyph(x, y, glyph.info, atlasTexture);

		cursorX += glyph.xAdvance;
		cursorY += glyph.yAdvance;
	}
}

int main()
{
	constexpr int FONT_SIZE = 64;
	const char* FONT_PATH = "fonts/Roboto-Regular.ttf";

	Trex::Atlas atlas(FONT_PATH, FONT_SIZE, Trex::Charset::Full());
	Trex::TextShaper shaper(atlas);
	Trex::ShapedGlyphs asciiGlyphs = shaper.ShapeAscii("Hello, World!");

	std::vector<uint32_t> somePolishText = {
		0x5A, 0x61, 0x17C, 0xF3, 0x142, 0x107,
		0x20, 0x67, 0x119, 0x15B, 0x6C, 0x105,
		0x20, 0x6A, 0x61, 0x17A, 0x144
	};
	Trex::ShapedGlyphs unicodeGlyphs = shaper.ShapeUnicode(somePolishText);

	InitWindow(600, 250, "TextShaping Example");

	// Load atlas texture
	Image atlasImage = GetAtlasAsBitmapImage(atlas);
	Texture2D atlasTexture = LoadTextureFromImage(atlasImage);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);

		// First line of text
		float cursorY = 100.0f;
		RenderShapedText(50, cursorY, asciiGlyphs, atlasTexture);

		// Second line of text
		cursorY += shaper.GetBaselineHeight();
		RenderShapedText(50, cursorY, unicodeGlyphs, atlasTexture);

		EndDrawing();
	}

	UnloadTexture(atlasTexture);
	CloseWindow();

	return 0;
}
