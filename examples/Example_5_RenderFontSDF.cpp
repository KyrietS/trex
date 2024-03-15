#include "Trex/Atlas.hpp"
#include "raylib.h"

Image GetAtlasAsBitmapImage(const Trex::Atlas::Bitmap& bitmap)
{
	Image atlasImage;
	atlasImage.data = (void*)bitmap.Data().data(); // pointer to the atlas bitmap data
	atlasImage.width = (int)bitmap.Width(); // width of the atlas bitmap
	atlasImage.height = (int)bitmap.Height(); // height of the atlas bitmap
	atlasImage.mipmaps = 1;
	atlasImage.format = bitmap.Channels() == 3 ? PIXELFORMAT_UNCOMPRESSED_R8G8B8 : PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
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
	const Trex::Atlas::Bitmap& bitmap = atlas.GetBitmap();
	const Trex::Atlas::Glyphs& glyphs = atlas.GetGlyphs();

	atlas.SaveToFile("Example_5_Atlas_SDF.png");

	InitWindow(500, 250, "RenderFontSDF Example");

	// Load atlas texture
	Image atlasImage = GetAtlasAsBitmapImage(bitmap);
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
		const Trex::Glyph& glyphA = glyphs.GetGlyphByCodepoint('a');
		RenderGlyph(50, 50, glyphA, atlasTexture);

		// Character '@'
		const Trex::Glyph& glyphAtSign = glyphs.GetGlyphByCodepoint('@');
		RenderGlyph(150, 50, glyphAtSign, atlasTexture);

		// Character from outside the ASCII charset
		const Trex::Glyph& glyphUndefined = glyphs.GetGlyphByCodepoint((char)178);
		RenderGlyph(250, 50, glyphUndefined, atlasTexture);

		EndShaderMode();
		EndMode2D();
		EndDrawing();
	}

	return 0;
}