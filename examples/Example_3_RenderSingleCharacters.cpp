#include "Atlas.hpp"
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

int main()
{
	constexpr int FONT_SIZE = 64;
	const char* FONT_PATH = "fonts/Roboto-Regular.ttf";

	Trex::Atlas atlas(FONT_PATH, FONT_SIZE, Trex::Charset::Ascii());

	InitWindow(500, 250, "RenderSingleCharacters Example");

	// Load atlas texture
	Image atlasImage = GetAtlasAsBitmapImage(atlas);
	Texture2D atlasTexture = LoadTextureFromImage(atlasImage);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);

		// Character 'a'
		const Trex::Glyph& glyphA = atlas.GetGlyphByCodepoint('a');
		RenderGlyph(50, 50, glyphA, atlasTexture);

		// Character '@'
		const Trex::Glyph& glyphAtSign = atlas.GetGlyphByCodepoint('@');
		RenderGlyph(150, 50, glyphAtSign, atlasTexture);

		// Character from outside the ASCII charset
		const Trex::Glyph& glyphUndefined = atlas.GetGlyphByCodepoint((char)178);
		RenderGlyph(250, 50, glyphUndefined, atlasTexture);

		EndDrawing();
	}

	UnloadTexture(atlasTexture);
	CloseWindow();

	return 0;
}
