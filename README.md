# Trex

Trex is a C++ library that can render the font with [FreeType](https://github.com/freetype/freetype) library, generate glyph atlas and shape the text with [HarfBuzz](https://github.com/harfbuzz/harfbuzz). It provides a simple API making it easy to integrate high-quality typography into you application.

The text rendered by this library is every pixel identical to that rendered by the Chrome and Firefox browsers. As such, Trex is an excellent choice when you need to display text in your application that, when exported to SVG or HTML format, should look identical.

## Features
* **Text Rendering** - Trex allows you to render text with FreeType, providing high-quality and accurate glyph rendering on various platforms.
* **Glyph Atlas Generation** - With Trex, you can generate efficient and minimal glyph atlases. Sides of a power of 2 are always used.
* **Text Shaping** - Trex integrates HarfBuzz to shape text, ensuring proper placement and shaping of complex scripts and languages.
* **High Performance** - The library is very fast as it relies on algorithms implemented in state-of-the-art FreeType and HarfBuzz libraries.
* **Platform Independent** - Trex is platform-independent and so are its dependencies.
* **Easy Integration** - The library provides a simple and minimalistic API, making it easy to integrate text rendering capabilities into you projects.
* **Static Library** - Trex uses CMake and it is configured as a static library.

## Basic Example

```cpp
Trex::Atlas atlas("arial.ttf", 32, Trex::Charset::Ascii());
atlas.SaveToFile("atlas.png");

Trex::TextShaper shaper(atlas);
Trex::ShapedGlyphs glyphs = shaper.ShapeAscii("Hello world!");

float cursorX = 100;
float cursorY = 100;
for (const auto& glyph : glyphs)
{
	float x = cursorX + glyph.xOffset + glyph.info.bearingX;
	float y = cursorY + glyph.yOffset - glyph.info.bearingY;

    int atlasGlyphX = glyph.info.x;
    int atlasGlyphY = glyph.info.y;
    int atlasGlyphWidth = glyph.info.width;
    int atlasGlyphHeight = glyph.info.height;

    // Draw a glyph at (x, y) by taking the atlas bitmap fragment
    // ...

    cursorX += glyph.xAdvance;
    cursorX += glyph.yAdvance;
}

```

## Getting Started
To get started with Trex, follow the instructions below:

1. Clone the repository recursively: `git clone --recurse-submodules https://github.com/KyrietS/trex.git`
2. Configure your project's CMake file to include the Trex library. The target exported from this library is called `trex`.
3. Build and link your project with the Trex library.
4. You can use the provided examples in the `examples` folder as a reference of how to use Trex.

## Dependencies

Trex has the following dependencies:

* FreeType - A high-quality font engine for rendering text.
* HarfBuzz - A text shaping engine for accurate and complex text shaping.

Make sure to clone these dependencies along with the Trex.

Examples use [raylib](https://github.com/raysan5/raylib) library to render a text on the screen.

## License
Copyright © 2023 KyrietS
Use of this software is granted under the terms of the MIT License.

See the [LICENSE](LICENSE) file for more details.