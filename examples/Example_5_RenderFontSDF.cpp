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
	DrawTextureRec(atlasTexture, atlasFragment, { x, y }, RED);
}

int main()
{
	constexpr int FONT_SIZE = 64;
	const char* FONT_PATH = "fonts/Roboto-Regular.ttf";
	const char* SHADER_PATH = "shaders/sdf.fs";

	Trex::Atlas atlas(FONT_PATH, FONT_SIZE, Trex::Charset::Ascii(), Trex::RenderMode::SDF);
	atlas.SaveToFile("RenderFontSDF_atlas.png");

	InitWindow(500, 250, "RenderFontSDF Example");

	// Load atlas texture
	Image atlasImage = GetAtlasAsBitmapImage(atlas);
	Texture2D atlasTexture = LoadTextureFromImage(atlasImage);
	SetTextureFilter(atlasTexture, TEXTURE_FILTER_BILINEAR); // required for SDF shader

	// Load SDF shader
	TraceLog(LOG_INFO, "Loading shader from %s", SHADER_PATH);
	Shader sdfShader = LoadShader(nullptr, SHADER_PATH);
	TraceLog(LOG_INFO, "Shader loaded successfully");

	Camera2D camera = { 0 };
	camera.zoom = 1.0f;
	camera.target = { 250, 125 };
	camera.offset = { 250, 125 };

	while (!WindowShouldClose())
	{
		DrawText("Use mouse wheel to zoom in-out, drag to move the camera", 10, 10, 10, DARKGRAY);
		camera.zoom *= 1.0f + (float)GetMouseWheelMove() * 0.05f;
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			camera.target.x -= GetMouseDelta().x / camera.zoom;
			camera.target.y -= GetMouseDelta().y / camera.zoom;
		}

		BeginDrawing();
		ClearBackground(WHITE);

		BeginMode2D(camera);
		BeginShaderMode(sdfShader);
		
		// Character 'a'
		const Trex::Glyph& glyphA = atlas.GetGlyphByCodepoint('a');
		RenderGlyph(50, 50, glyphA, atlasTexture);

		// Character '@'
		const Trex::Glyph& glyphAtSign = atlas.GetGlyphByCodepoint('@');
		RenderGlyph(150, 50, glyphAtSign, atlasTexture);

		// Character from outside the ASCII charset
		const Trex::Glyph& glyphUndefined = atlas.GetGlyphByCodepoint((char)178);
		RenderGlyph(250, 50, glyphUndefined, atlasTexture);

		EndShaderMode();
		EndMode2D();
		EndDrawing();
	}

	return 0;
}