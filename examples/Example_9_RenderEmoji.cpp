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
#ifdef _WIN32
	// Windows native emojis
	const char* FONT_PATH = "C:\\Windows\\Fonts\\seguiemj.ttf";
#else
	const char* FONT_PATH = "fonts/OpenMoji.ttf";
#endif

	Trex::Atlas atlas( FONT_PATH, FONT_SIZE, Trex::Charset::Full(), Trex::RenderMode::COLOR );
	const Trex::Atlas::Bitmap& bitmap = atlas.GetBitmap();
	const Trex::Atlas::Glyphs& glyphs = atlas.GetGlyphs();

	atlas.SaveToFile( "Example_9_Atlas_Emoji.png" );

	InitWindow( 300, 125, "Render Emojis Example" );

	// Load atlas texture
	Image atlasImage = GetAtlasBitmapAsImage( bitmap );
	Texture2D atlasTexture = LoadTextureFromImage( atlasImage );

	while( !WindowShouldClose() )
	{
		BeginDrawing();
		ClearBackground( WHITE );

		// Smile emoji
		const Trex::Glyph& glyphSmile = glyphs.GetGlyphByCodepoint( 0x1F604 );
		RenderGlyph( 25, 25, glyphSmile, atlasTexture );

		// Hearth emoji
		const Trex::Glyph& glyphAtSign = glyphs.GetGlyphByCodepoint( 0x2764 );
		RenderGlyph( 100, 25, glyphAtSign, atlasTexture );

		// Hedgehog emoji
		const Trex::Glyph& glyphUndefined = glyphs.GetGlyphByCodepoint( 0x1F994 );
		RenderGlyph( 175, 25, glyphUndefined, atlasTexture );

		EndDrawing();
	}

	UnloadTexture( atlasTexture );
	CloseWindow();

	return 0;
}
