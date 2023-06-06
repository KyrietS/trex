# Trex API Documentation

## Table of Contents
1. [Font](#font)
3. [Charset](#charset)
3. [Glyph](#glyph)
3. [RenderMode](#rendermode)
2. [AtlasBitmap](#atlasbitmap)
2. [AtlasGlyphs](#atlasglyphs)
2. [Atlas](#atlas)
4. [ShapedGlyph](#shapedglyph)
4. [ShapedGlyphs](#shapedglyphs)
3. [TextShaper](#textshaper)

## Font
Used internally by [Atlas](#atlas) to load a font file and generate a bitmap.

### Font::Font
```cpp
Font::Font(const char* path);
```
* `path` - Path to the font file.

### Font::SetSize
```cpp
void Font::SetSize(Pixels size);
void Font::SetSize(Points size);
```
Change font size. The default font size after loading is 12pt.
* `size` - Size of the font in pixels or points.

Note: After the atlas is generated, the font size must not be changed.

### GetGlyphIndex
```cpp
uint32_t Font::GetGlyphIndex(uint32_t codepoint) const;
```
Get the glyph index of a codepoint. Returns 0 if the codepoint is not supported by the font.
* `codepoint` - Unicode codepoint.

## Charset
Represents a set of supported codepoints.

### Charset::Charset
```cpp
explicit Charset(std::string codepoints);
explicit Charset(uint32_t last); // first = 0
explicit Charset(uint32_t first, uint32_t last);
explicit Charset(std::vector<uint32_t> codepoints);
```
* `codepoints` - A string of 1-byte codepoints.
* `first` - First codepoint in the range.
* `last` - Last codepoint in the range.
* `codepoints` - A vector of codepoints.

### Charset::Full
```cpp
static Charset Charset::Full();
```
Returns a charset containing all possible codepoints.

Note: Charset does not know what codepoints are supported by the font. So it is impossible to count the number of supported codepoints. It will be determined during the atlas generation for the given font.

### Charset::Ascii
```cpp
static Charset Charset::Ascii();
```
Returns a charset containing ASCII codepoints (0-127).

### Charset::Size
```cpp
size_t Charset::Size() const;
```
Returns the number of codepoints in the charset.

### Charset::Codepoints
```cpp
const std::vector<uint32_t>& Charset::Codepoints() const;
```
Returns a vector of codepoints in the charset.

Note: If the charset is marked as "full", the vector will be empty because it is impossible to list all possible codepoints without knowing the font.

### Charset::IsFull
```cpp
bool Charset::IsFull() const;
```
Returns true if the charset contains all possible codepoints.

## Glyph
Represents a glyph in the atlas.
```cpp
struct Glyph
{
    int codepoint;
    int glyphIndex;
    int x, y;
    int width, height,
    int bearingX, bearingY;
};
```
* `codepoint` - Unicode codepoint.
* `glyphIndex` - Glyph index in the font.
* `x` - X coordinate of the glyph in the atlas.
* `y` - Y coordinate of the glyph in the atlas.
* `width` - Width of the glyph in the atlas.
* `height` - Height of the glyph in the atlas.
* `bearingX` - Horizontal bearing of the glyph.
* `bearingY` - Vertical bearing of the glyph.

## RenderMode
Specifies how the text should be rendered.
```cpp
enum class RenderMode
{
    DEFAULT,
    SDF
};
```
* `DEFAULT` - Rasterize the text with the default, grayscale FreeType renderer.
* `SDF` - rasterize the text with the SDF renderer. You will need a fragment shader to display the text properly.

## AtlasBitmap
Represents a bitmap of the atlas.
```cpp
using AtlasBitmap = std::vector<uint8_t>;
```

## AtlasGlyphs
Represents a map of [Glyph](#glyph)s. The key is the glyph index.
```cpp
using AtlasGlyphs = std::map<uint32_t, Glyph>;
```

## Atlas
Represents a glyph atlas.

### Atlas::Atlas
```cpp
Atlas(const std::string& fontPath, int fontSize, const Charset&, RenderMode, int padding);
```
* `fontPath` - Path to the font file.
* `fontSize` - Size of the font in pixels.
* `charset` - Charset of the atlas. See: [Charset](#charset).
* `renderMode` - Render mode of the atlas. See: [RenderMode](#rendermode).
* `padding` - Padding between glyphs in the atlas. Default is 1.

### Atlas::SaveToFile
```cpp
void Atlas::SaveToFile(const std::string& path) const;
```
Save the atlas bitmap to a PNG or BMP file.
* `path` - Path to the file. The file extension determines the format. It must be one of: `.png`, `.bmp`.

### Atlas::SetDefaultGlyph
```cpp
void Atlas::SetDefaultGlyph(uint32_t codepoint);
```
Set the default glyph to be used when a glyph is not found in the atlas.
* `codepoint` - Unicode codepoint. If the codepoint is not found in the atlas, the function does nothing.

If the default glyph is not set, the atlas will try to set it to some sensible value. 

### Atlas::GetGlyphByCodepoint
```cpp
const Glyph& Atlas::GetGlyphByCodepoint(uint32_t codepoint) const;
```
Get a [Glyph](#glyph) by its codepoint. If the glyph is not found, the default glyph is returned.
* `codepoint` - Unicode codepoint.

### Atlas::GetGlyphByIndex
```cpp
const Glyph& Atlas::GetGlyphByIndex(uint32_t glyphIndex) const;
```
Get a [Glyph](#glyph) by its glyph index. If the glyph is not found, the default glyph is returned.
* `glyphIndex` - Glyph index.

### Atlas::GetBitmap
```cpp
const AtlasBitmap& Atlas::GetBitmap() const;
AtlasBitmap& GetBitmap();
```
Get the atlas bitmap. See: [AtlasBitmap](#atlasbitmap).

### Atlas::GetWidth
```cpp
int Atlas::GetWidth() const;
```
Get the width of the atlas bitmap.

### Atlas::GetHeight
```cpp
int Atlas::GetHeight() const;
```
Get the height of the atlas bitmap.

### Atlas::UnloadBitmap
```cpp
void Atlas::UnloadBitmap();
```
Unload the atlas bitmap from memory.

### Atlas::GetFontFace
```cpp
const FT_FaceRec_* Atlas::GetFontFace() const;
```
Get the FreeType font face.

## ShapedGlyph
Represents a shaped glyph.
```cpp
struct ShapedGlyph
{
    float xOffset;
    float yOffset;
    float xAdvance;
    float yAdvance;

    Glyph info;
};
```
* `xOffset` - Horizontal offset of the glyph.
* `yOffset` - Vertical offset of the glyph.
* `xAdvance` - Horizontal advance of the glyph.
* `yAdvance` - Vertical advance of the glyph.
* `info` - [Glyph](#glyph) info object.

### ShapedGlyphs
Represents a vector of [ShapedGlyph](#shaped-glyph)s.
```cpp
using ShapedGlyphs = std::vector<ShapedGlyph>;
```

## TextShaper
Used to shape text into [ShapedGlyphs](#shaped-glyphs).

### TextShaper::TextShaper
```cpp
TextShaper::TextShaper(const Atlas& atlas);
```
* `atlas` - [Atlas](#atlas) object.

### TextShaper::ShapeAscii
```cpp
ShapedGlyphs TextShaper::ShapeAscii(const std::string& text);
```
Shape ASCII text into [ShapedGlyphs](#shaped-glyphs).
* `text` - ASCII text.

### TextShaper::ShapeUtf8
```cpp
ShapedGlyphs TextShaper::ShapeUtf8(const std::string& text);
```
Shape UTF-8 text into [ShapedGlyphs](#shaped-glyphs).
* `text` - byte sequence of UTF-8 encoded text.

## TextShaper::ShapeUtf32
```cpp
ShapedGlyphs TextShaper::ShapeUtf32(const std::u32string& text);
```
Shape UTF-32 text into [ShapedGlyphs](#shaped-glyphs).
* `text` - UTF-32 encoded text. Each character is a Unicode codepoint.

Note: u32string must be converted to a vector of uint32_t before shaping. It allocates additional memory and copies the string. Use ShapeUnicode function whenever possible.

### TextShaper::ShapeUnicode
```cpp
ShapedGlyphs TextShaper::ShapeUnicode(const std::vector<uint32_t>& codepoints);
```
Shape Unicode text into [ShapedGlyphs](#shaped-glyphs).
* `codepoints` - Unicode codepoints.

