#include "Trex/Atlas.hpp"
#include "raylib.h"

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

	Trex::Atlas atlas( FONT_PATH, FONT_SIZE, Trex::Charset::Ascii(), Trex::RenderMode::LCD );

	atlas.SaveToFile( "Example_8_RenderFontSubpixel.png" );

	InitWindow( 500, 250, "RenderSingleCharacters Example" );

	// Load atlas texture
	Image atlasImage = GetAtlasBitmapAsImage( atlas.GetBitmap() );
	Texture2D atlasTexture = LoadTextureFromImage( atlasImage );

	while( !WindowShouldClose() )
	{
		BeginDrawing();
		ClearBackground( WHITE );

		// Character 'a'
		const Trex::Glyph& glyphA = atlas.GetGlyphByCodepoint( 'a' );
		RenderGlyph( 50, 50, glyphA, atlasTexture );

		// Character '@'
		const Trex::Glyph& glyphAtSign = atlas.GetGlyphByCodepoint( '@' );
		RenderGlyph( 150, 50, glyphAtSign, atlasTexture );

		// Character from outside the ASCII charset
		const Trex::Glyph& glyphUndefined = atlas.GetGlyphByCodepoint( (char)178 );
		RenderGlyph( 250, 50, glyphUndefined, atlasTexture );

		EndDrawing();
	}

	UnloadTexture( atlasTexture );
	CloseWindow();

	return 0;
}
