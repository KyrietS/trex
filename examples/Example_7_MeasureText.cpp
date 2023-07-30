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
	Trex::ShapedGlyphs shapedGlyphs = shaper.ShapeAscii("Hello, World!");
	Trex::TextMeasurement textMeas = Trex::TextShaper::Measure(shapedGlyphs);

	InitWindow(600, 250, "MeasureText Example");

	// Load atlas texture
	Image atlasImage = GetAtlasAsBitmapImage(atlas);
	Texture2D atlasTexture = LoadTextureFromImage(atlasImage);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);

		RenderShapedText(50, 100, shapedGlyphs, atlasTexture);

		DrawCircle(50, 100, 3, VIOLET);

		// Draw cursor before the text
		DrawRectangle(50, 100 + (int)textMeas.yOffset, 1, (int)textMeas.height, RED);

		// Draw filled rectangle over the text
		DrawRectangle(50 + (int)textMeas.xOffset, 100 + (int)textMeas.yOffset, (int)textMeas.width, (int)textMeas.height, Fade(GREEN, 0.2f));

		// Draw cursor after the text
		DrawRectangle(50 + (int)textMeas.xAdvance, 100 + (int)textMeas.yAdvance + (int)textMeas.yOffset, 1, (int)textMeas.height, BLUE);

		EndDrawing();
	}

	UnloadTexture(atlasTexture);
	CloseWindow();

	return 0;
}
