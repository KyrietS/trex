#include "Trex/Atlas.hpp"
#include "raylib.h"

const char* fragmentShader = R"(#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor; // background color

// Input uniform values
uniform sampler2D texture0; // texture

// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 fontColor = vec4(0.0, 0.0, 0.0, 1.0); // black font
    vec4 background = vec4(1.0, 1.0, 1.0, 1.0); // white background
    vec4 texture = texture(texture0, fragTexCoord).rgba; // texture fragment with glyph

    finalColor.r = texture.r * fontColor.r * fontColor.a + background.r * (1.0 - texture.r * fontColor.a);
    finalColor.g = texture.g * fontColor.g * fontColor.a + background.g * (1.0 - texture.g * fontColor.a);
    finalColor.b = texture.b * fontColor.b * fontColor.a + background.b * (1.0 - texture.b * fontColor.a);

    // I don't want to draw a background behind the glyph
    // So the pixels that don't emit any color are transparent
    if (texture.rgb == vec3(0.0, 0.0, 0.0))
    {
        finalColor.a = 0.0;
    }
    else 
    {
        finalColor.a = fontColor.a;
    }
})";

Image GetAtlasBitmapAsImage( const Trex::Atlas::Bitmap& bitmap )
{
	Image atlasImage;
	atlasImage.data = (void*)bitmap.Data().data(); // pointer to the atlas bitmap data
	atlasImage.width = (int)bitmap.Width(); // width of the atlas bitmap
	atlasImage.height = (int)bitmap.Height(); // height of the atlas bitmap
	atlasImage.mipmaps = 1;
	atlasImage.format = bitmap.Channels() == 3 ? PIXELFORMAT_UNCOMPRESSED_R8G8B8 : PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
	return atlasImage;
}

void RenderGlyph( float x, float y, const Trex::Glyph& glyph, Texture2D& atlasTexture )
{
	Rectangle atlasFragment = {
		.x = (float)glyph.x, // top-left corner of the glyph in the atlas
		.y = (float)glyph.y, // top-left corner of the glyph in the atlas
		.width = (float)glyph.width, // width of the glyph in the atlas
		.height = (float)glyph.height // height of the glyph in the atlas
	};

	// Draw a texture fragment from the atlas
	DrawTextureRec( atlasTexture, atlasFragment, { x, y }, WHITE );
}

int main()
{
	constexpr int FONT_SIZE = 64;
	const char* FONT_PATH = "fonts/Roboto-Regular.ttf";
	const char* SHADER_PATH = "shaders/lcd.fs";

	Trex::Atlas atlas( FONT_PATH, FONT_SIZE, Trex::Charset::Ascii(), Trex::RenderMode::LCD );
	const Trex::Atlas::Bitmap& bitmap = atlas.GetBitmap();
	const Trex::Atlas::Glyphs& glyphs = atlas.GetGlyphs();

	atlas.SaveToFile( "Example_8_Atlas_Subpixel.png" );

	InitWindow( 500, 250, "Subpixel Rendering Example" );

	// Load atlas texture
	Image atlasImage = GetAtlasBitmapAsImage( bitmap );
	Texture2D atlasTexture = LoadTextureFromImage( atlasImage );
	Shader lcdShader = LoadShaderFromMemory( nullptr, fragmentShader );
	//Shader lcdShader = LoadShader( nullptr, SHADER_PATH );

	while( !WindowShouldClose() )
	{
		BeginDrawing();
		ClearBackground( WHITE );
		BeginShaderMode( lcdShader );

		// Character 'a'
		const Trex::Glyph& glyphA = glyphs.GetGlyphByCodepoint( 'a' );
		RenderGlyph( 50, 50, glyphA, atlasTexture );

		// Character '@'
		const Trex::Glyph& glyphAtSign = glyphs.GetGlyphByCodepoint( '@' );
		RenderGlyph( 150, 50, glyphAtSign, atlasTexture );

		// Character from outside the ASCII charset
		const Trex::Glyph& glyphUndefined = glyphs.GetGlyphByCodepoint( (char)178 );
		RenderGlyph( 250, 50, glyphUndefined, atlasTexture );

		EndShaderMode();
		EndDrawing();
	}

	UnloadTexture( atlasTexture );
	CloseWindow();

	return 0;
}
