#include "Atlas.hpp"
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

Image GetLetterFromAtlasImage(Trex::Atlas& atlas, char letter)
{
	Trex::Glyph glyph = atlas.GetGlyphByCodepoint(letter);

	Image atlasAsImage = GetAtlasAsBitmapImage(atlas);
	Rectangle atlasFragment = {
		.x = (float)glyph.x, // top-left corner of the glyph in the atlas
		.y = (float)glyph.y, // top-left corner of the glyph in the atlas
		.width = (float)glyph.width, // width of the glyph in the atlas
		.height = (float)glyph.height // height of the glyph in the atlas
	};

	return ImageFromImage(atlasAsImage, atlasFragment);
}

int main()
{
	constexpr int FONT_SIZE = 64;
	const char* FONT_PATH = "fonts/Roboto-Regular.ttf";

	Trex::Atlas atlas(FONT_PATH, FONT_SIZE, Trex::Charset::Ascii());

	InitWindow(500, 250, "RenderSingleCharacters Example");

	// Character 'a'
	Image letterA = GetLetterFromAtlasImage(atlas, 'a');
	Texture2D letterATexture = LoadTextureFromImage(letterA);
	UnloadImage(letterA);

	// Character '@'
	Image atSign = GetLetterFromAtlasImage(atlas, '@');
	Texture2D atSignTexture = LoadTextureFromImage(atSign);
	UnloadImage(atSign);

	// Character from outside the ASCII charset
	Image undefinedCharacter = GetLetterFromAtlasImage(atlas, (char)178);
	Texture2D undefinedCharacterTexture = LoadTextureFromImage(undefinedCharacter);
	UnloadImage(undefinedCharacter);

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(WHITE);
		DrawTexture(letterATexture, 50, 50, WHITE);
		DrawTexture(atSignTexture, 150, 50, WHITE);
		DrawTexture(undefinedCharacterTexture, 250, 50, WHITE);
		EndDrawing();
	}

	UnloadTexture(letterATexture);
	UnloadTexture(atSignTexture);
	UnloadTexture(undefinedCharacterTexture);
	CloseWindow();

	return 0;
}
