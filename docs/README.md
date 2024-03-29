# Trex API Documentation

## Table of Contents
1. [Font](#font)
2. [FontMetrics](#fontmetrics)
3. [Charset](#charset)
4. [Glyph](#glyph)
5. [RenderMode](#rendermode)
6. [Atlas](#atlas)
7. [Atlas::Glyphs](#atlasglyphs)
8. [Atlas::Bitmap](#atlasbitmap)
9. [ShapedGlyph](#shapedglyph)
10. [ShapedGlyphs](#shapedglyphs)
11. [TextMeasurement](#textmeasurement)
12. [TextShaper](#textshaper)
13. [BitmapHelpers](#bitmaphelpers)

## Font
Used internally by [Atlas](#atlas) to load a font file and generate a bitmap.

### Font::Font
```cpp
Font::Font(const char* path);
Font::Font(std::span<const uint8_t> data);
```
* `path` - Path to the font file.
* `data` - Font file data. This span should represent contiguous array of bytes.

Note: `data` is copied into the font object. It is safe to destroy the original data after the font is created.

### Font::SetSize
```cpp
using FontSize = std::variant<Pixels, Points>;

void Font::SetSize(const FontSize& size);
```
Change font size. The default font size after loading is 12pt.
* `size` - Size of the font in pixels or points.

Note: After the atlas is generated, the font size must not be changed.

### Font::GetGlyphIndex
```cpp
uint32_t Font::GetGlyphIndex(uint32_t codepoint) const;
```
Get the glyph index of a codepoint. Returns 0 if the codepoint is not supported by the font.
* `codepoint` - Unicode codepoint.

### Font::GetMetrics
```cpp
FontMetrics Font::GetMetrics() const;
```
Get the font metrics. See: [FontMetrics](#fontmetrics).

## FontMetrics
Represents the metrics of a font.
```cpp
struct FontMetrics
{
    int ascender;
    int descender;
    int height;
};
```
* `ascender` - Max distance (in pixels) from baseline to top of a glyph (positive value)
* `descender` - Max distance (in pixels) from baseline to the bottom of a glyph (negative value)
* `height` - Distance (in pixels) from one baseline to the next.

These values are always scaled according to the font size and they are rounded to the nearest integer.

## Charset
Represents a set of supported codepoints.

### Charset::Charset
```cpp
using Range = std::pair<uint32_t, uint32_t>;

explicit Charset(); // empty charset
explicit Charset(uint32_t first, uint32_t last);
explicit Charset(const std::vector<Range> codepointRanges);
explicit Charset(std::span<const Range> codepointRanges);
```
* `first` - First codepoint in the range.
* `last` - Last codepoint in the range.
* `codepointRanges` - An array of pairs `first` and `last`.

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
const std::set<uint32_t>& Charset::Codepoints() const;
```
Returns a vector of codepoints in the charset.

Note: If the charset is marked as "full", the vector will be empty because it is impossible to list all possible codepoints without knowing the font.

### Charset::IsFull
```cpp
bool Charset::IsFull() const;
```
Returns true if the charset contains all possible codepoints.

### Charset::begin/end
This struct is iterable and thus can be used in range-based for loops. It iterates over all codepoints in the charset.

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
    COLOR,
    SDF,
    LCD
};
```
* `DEFAULT` - Rasterize the text with the default, grayscale FreeType renderer. The bitmap will have 1-byte color channel.
* `COLOR` - Rasterize the text with colors if the font supports it. The bitmap will have 4-byte color channels in RGBA format.
* `SDF` - rasterize the text with the SDF renderer. You will need a fragment shader to display the text properly. The bitmap will have 1-byte color channel.
* `LCD` - rasterize the text with the subpixel renderer. The bitmap will have 3 color channels and the bitmap will be in RGB format.

## Atlas
Represents aa atlas of glyphs.

### Atlas::Atlas
```cpp
Atlas(const std::string& fontPath, int fontSize, const Charset&, RenderMode, int padding);
Atlas(std::span<const uint8_t> fontData, int fontSize, const Charset&, RenderMode, int padding);
```
* `fontPath` - Path to the font file.
* `fontSize` - Size of the font in pixels.
* `charset` - Charset of the atlas. Default is `Full`. See: [Charset](#charset).
* `renderMode` - Render mode of the atlas. Default is `DEFAULT`. See: [RenderMode](#rendermode).
* `padding` - Padding between glyphs in the atlas. Default is `1`.
* `fontData` - Font file data. This span should represent contiguous array of bytes.

Note: `Charset` and `fontData` are copied and then owned by the atlas. They can be safely destroyed after the atlas is created.

### Atlas::GetBitmap
```cpp
const Atlas::Bitmap& Atlas::GetBitmap() const;
```
Get the atlas bitmap. See: [Atlas::Bitmap](#atlasbitmap).

### Atlas::GetGlyphs
```cpp
const Atlas::Glyphs& Atlas::GetGlyphs() const;
```
Get all glyphs data. See: [Atlas::Glyphs](#atlasglyphs).\
Note: If you use [TextShaper](#textshaper) to shape text, you don't need to use this function. All needed glyph data is already stored in the [ShapedGlyph](#shapedglyph)::info.

### Atlas::GetFont
```cpp
const shared_ptr<const Font> Atlas::GetFontFace() const;
```
Get the Font object. 

Note: You should never use the `Font::face` without making sure that the Font object is still alive.

### Atlas::SaveToFile
```cpp
void Atlas::SaveToFile(const std::string& path) const;
```
Save the atlas bitmap to a PNG or BMP file.
* `path` - Path to the file. The file extension determines the format. It must be one of: `.png`, `.bmp`.

## Atlas::Glyphs
Represents all rendered glyphs in the atlas.

### Atlas::Glyphs::SetUnknownGlyph
```cpp
void Atlas::Glyphs::SetUnknownGlyph(uint32_t codepoint) const;
```
Set the unknown glyph to be used when a glyph is not found in the atlas.
* `codepoint` - Unicode codepoint. If the codepoint is not found in the atlas, the function does nothing.

If the unknown glyph is not set, the atlas will try to set it to some sensible value. 

### Atlas::Glyphs::GetUnknownGlyph
```cpp
const Glyph& Atlas::Glyphs::GetUnknownGlyph() const;
```
Get the unknown glyph.

### Atlas::Glyphs::GetGlyphByCodepoint
```cpp
const Glyph& Atlas::Glyphs::GetGlyphByCodepoint(uint32_t codepoint) const;
```
Get a [Glyph](#glyph) by its codepoint. If the glyph is not found, the default glyph is returned.
* `codepoint` - Unicode codepoint.

### Atlas::Glyphs::GetGlyphByIndex
```cpp
const Glyph& Atlas::Glyphs::GetGlyphByIndex(uint32_t glyphIndex) const;
```
Get a [Glyph](#glyph) by its glyph index. If the glyph is not found, the default glyph is returned.
* `glyphIndex` - Glyph index.

### Atlas::Glyphs::Add
```cpp
void Atlas::Glyphs::Add(int x, int y, const FreeTypeGlyph&);
```
Add new glyph. **Internal use only.**

## Atlas::Bitmap
Represents the rendered atlas bitmap with all glyphs.

### Atlas::Bitmap::GetWidth
```cpp
unsigned int Atlas::Bitmap::Width() const;
```
Get the width of the atlas bitmap.

### Atlas::Bitmap::GetHeight
```cpp
unsigned int Atlas::Bitmap::Height() const;
```
Get the height of the atlas bitmap.

### Atlas::Bitmap::GetChannels
```cpp
unsigned int Atlas::Bitmap::Channels() const;
```
Get the number of color channels in the atlas bitmap. When `RenderMode::DEFAULT` or `RenderMode::SDF` is used it is always 1. When `RenderMode::SDF` is used it is always 3 and the bitmap is in RGB format.

### Atlas::Bitmap::Format
```cpp
PixelFormat Atlas::Bitmap::Format() const;
```
Get the [PixelFormat](#pixelformat) of the atlas bitmap. 

### Atlas::Bitmap::Draw
```cpp
void Atlas::Bitmap::Draw(int x, int y, const FreeTypeGlyph&);
```
Draw a glyph into the atlas bitmap. **Internal use only.**

### ShapedGlyph
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

### ShapedGlyphs
Represents a vector of [ShapedGlyph](#shapedglyph)s.
```cpp
using ShapedGlyphs = std::vector<ShapedGlyph>;
```

### TextMeasurement
Represents the dimension of a shaped text.
```cpp
struct TextMeasurement
{
    float width, height;
    float xOffset, yOffset;
    float xAdvance, yAdvance;
};
```
* `width` - Width of the bounding box of the text.
* `height` - Height of the bounding box of the text.
* `xOffset` - Offset from the baseline origin to the leftmost edge of the bounding box.
* `yOffset` - Offset from the baseline origin to the topmost edge of the bounding box.
* `xAdvance` - Advance from the baseline origin to the end of the text (including advance of the last glyph).
* `yAdvance` - Advance from the baseline origin to the end of the text (including advance of the last glyph).

## TextShaper
Used to shape text into [ShapedGlyphs](#shapedglyphs).

### TextShaper::TextShaper
```cpp
TextShaper::TextShaper(const Atlas& atlas);
```
* `atlas` - [Atlas](#atlas) object. Can be cafely destroyed after the TextShaper is created.

### TextShaper::ShapeAscii
```cpp
ShapedGlyphs TextShaper::ShapeAscii(std::span<const char> text);
```
Shape ASCII text into [ShapedGlyphs](#shapedglyphs). This is alias to `ShapeUtf8`. All ASCII characters are already UTF-8 encoded.
* `text` - ASCII text.

### TextShaper::ShapeUtf8
```cpp
ShapedGlyphs TextShaper::ShapeUtf8(std::span<const char> text);
```
Shape UTF-8 text into [ShapedGlyphs](#shapedglyphs).
* `text` - byte sequence of UTF-8 encoded text.

### TextShaper::ShapeUtf32
```cpp
ShapedGlyphs TextShaper::ShapeUtf32(std::span<const char32_t> text);
```
Shape UTF-32 text into [ShapedGlyphs](#shapedglyphs).
* `text` - UTF-32 encoded text. Each character is a Unicode codepoint.

Note: char32_t* text must be converted to a vector of uint32_t before shaping. It allocates additional memory and copies the string. Use ShapeUnicode function whenever possible.

### TextShaper::ShapeUnicode
```cpp
ShapedGlyphs TextShaper::ShapeUnicode(std::span<const uint32_t> codepoints);
```
Shape Unicode text into [ShapedGlyphs](#shapedglyphs).
* `codepoints` - Unicode codepoints.

### TextShaper::GetFontMetrics
```cpp
FontMetrics TextShaper::GetFontMetrics() const;
```
Get the font metrics. See: [FontMetrics](#fontmetrics).

### TextShaper::Measure
```cpp
TextMeasurement TextShaper::Measure(const ShapedGlyphs& glyphs);
```
Measure the dimensions of a shaped text. Returns a [TextMeasurement](#textmeasurement) object.
* `glyphs` - [ShapedGlyphs](#shapedglyphs).

## BitmapHelpers
Helper functions for converting bitmaps to other formats. Trex uses 1-byte grayscale bitmaps and always returns a bitmap in this format.

### ConvertBitmapToGrayAlpha
```cpp
std::vector<uint8_t> ConvertBitmapToGrayAlpha(std::span<const uint8_t> input);
```
Convert 1-byte: GRAY8 to 2-byte: GRAYALPHA88.
* `input` - Input 1-byte grayscale bitmap.

### ConvertBitmapToRGB
```cpp
std::vector<uint8_t> ConvertBitmapToRGB(std::span<const uint8_t> input);
```
Convert 1-byte: GRAY8 to 3-byte: RGB888.
* `input` - Input 1-byte grayscale bitmap.

### ConvertBitmapToRGBA
```cpp
std::vector<uint8_t> ConvertBitmapToRGBA(std::span<const uint8_t> input);
```
Convert 1-byte: GRAY8 to 4-byte: RGBA8888.
* `input` - Input 1-byte grayscale bitmap.
