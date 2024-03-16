#include "Trex/Atlas.hpp"
#include "raylib.h"

Image GetAtlasBitmapAsImage( const Trex::Atlas::Bitmap& bitmap )
{
	Image atlasImage;
	atlasImage.data = (void*)bitmap.Data().data(); // pointer to the atlas bitmap data
	atlasImage.width = (int)bitmap.Width(); // width of the atlas bitmap
	atlasImage.height = (int)bitmap.Height(); // height of the atlas bitmap
	atlasImage.mipmaps = 1;
	switch( bitmap.Channels() )
	{
	case 1: atlasImage.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE; break;
	case 3: atlasImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8; break;
	case 4: atlasImage.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8; break;
	default: throw "unknown format";
	}
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
	const char* FONT_PATH = "C:\\Windows\\Fonts\\seguiemj.ttf";
	//const char* FONT_PATH = "fonts/Roboto-Regular.ttf";

	Trex::Atlas atlas( FONT_PATH, FONT_SIZE, Trex::Charset::Full(), Trex::RenderMode::COLOR );
	const Trex::Atlas::Bitmap& bitmap = atlas.GetBitmap();
	const Trex::Atlas::Glyphs& glyphs = atlas.GetGlyphs();

	atlas.SaveToFile( "Example_9_Atlas_Emoji.png" );

	InitWindow( 500, 250, "Render Emojis Example" );

	// Load atlas texture
	Image atlasImage = GetAtlasBitmapAsImage( bitmap );
	Texture2D atlasTexture = LoadTextureFromImage( atlasImage );

	while( !WindowShouldClose() )
	{
		BeginDrawing();
		ClearBackground( WHITE );

		// Smile emoji
		const Trex::Glyph& glyphSmile = glyphs.GetGlyphByCodepoint( 0x1F604 );
		RenderGlyph( 50, 50, glyphSmile, atlasTexture );

		// Character '@'
		const Trex::Glyph& glyphAtSign = glyphs.GetGlyphByCodepoint( '@' );
		RenderGlyph( 150, 50, glyphAtSign, atlasTexture );

		// Character from outside the ASCII charset
		const Trex::Glyph& glyphUndefined = glyphs.GetGlyphByCodepoint( (char)178 );
		RenderGlyph( 250, 50, glyphUndefined, atlasTexture );

		EndDrawing();
	}

	UnloadTexture( atlasTexture );
	CloseWindow();

	return 0;
}
