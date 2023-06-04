#include "Atlas.hpp"
#include "Shaper.hpp"
#include "raylib.h"

Image GetAtlasAsBitmapImage(Trex::Atlas& atlas)
{
	Image atlasImage;
	atlasImage.data = atlas.GetBitmap().data(); // pointer to the atlas bitmap data
	atlasImage.width = atlas.GetWidth(); // width of the atlas bitmap
	atlasImage.height = atlas.GetHeight(); // height of the atlas bitmap
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
		float x = cursorX + glyph.xOffset + glyph.info.bearingX;
		float y = cursorY + glyph.yOffset - glyph.info.bearingY;

		RenderGlyph(x, y, glyph.info, atlasTexture);

		cursorX += glyph.xAdvance;
		cursorX += glyph.yAdvance;
	}
}

int main()
{
	constexpr int FONT_SIZE = 64;
	const char* FONT_PATH = "fonts/Roboto-Regular.ttf";

	Trex::Atlas atlas(FONT_PATH, FONT_SIZE, Trex::Charset::Ascii());
	Trex::TextShaper shaper(atlas);
	Trex::ShapedGlyphs glyphs = shaper.ShapeAscii("Hello, World!");


	InitWindow(500, 250, "BasicTextShaping Example");

	// Load atlas texture
	Image atlasImage = GetAtlasAsBitmapImage(atlas);
	Texture2D atlasTexture = LoadTextureFromImage(atlasImage);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);

		RenderShapedText(50, 100, glyphs, atlasTexture);

		EndDrawing();
	}

	UnloadTexture(atlasTexture);
	CloseWindow();

	return 0;
}
