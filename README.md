# Trex

![trex-banner](https://github.com/KyrietS/trex/assets/19170699/8ed15586-9545-4b98-9e83-ce827bac6f3f)
[![Build](https://github.com/KyrietS/trex/actions/workflows/build.yml/badge.svg)](https://github.com/KyrietS/trex/actions/workflows/build.yml)
[![Tests](https://github.com/KyrietS/trex/actions/workflows/tests.yml/badge.svg)](https://github.com/KyrietS/trex/actions/workflows/tests.yml)
[![Examples](https://github.com/KyrietS/trex/actions/workflows/examples.yml/badge.svg)](https://github.com/KyrietS/trex/actions/workflows/examples.yml)
[![Lincense](https://img.shields.io/github/license/KyrietS/trex)](LICENSE)
[![Trex release](https://img.shields.io/github/v/release/KyrietS/trex?include_prereleases&sort=semver)](https://github.com/KyrietS/trex/releases)

Trex is a font rasterizer, atlas generator and text shaping library written in C++. It uses [FreeType](https://github.com/freetype/freetype) and [HarfBuzz](https://github.com/harfbuzz/harfbuzz) libraries under the hood. It provides a simple API that makes it easy to integrate high quality typography into your application.

The text rendered by this library is every pixel identical to that rendered by the Chrome and Firefox browsers. As such, Trex is an excellent choice when you need to display text in your application that, when exported to SVG or HTML format, should look identical.

## Features
* **Text Rendering** - Trex allows you to render text with FreeType, providing high-quality and accurate glyph rendering on various platforms.
* **Glyph Atlas Generation** - With Trex, you can generate efficient and minimal glyph atlases. Sides of a power of 2 are always used.
* **Text Shaping** - Trex integrates HarfBuzz to shape text, ensuring proper placement and shaping of complex scripts and languages.
* **High Performance** - The library is very fast as it relies on algorithms implemented in state-of-the-art FreeType and HarfBuzz libraries.
* **Platform Independent** - Trex is platform-independent and so are its dependencies.
* **Easy Integration** - The library provides a simple and minimalistic API, making it easy to integrate text rendering capabilities into you projects.
* **Static Library** - Trex uses CMake and it is configured as a static library.
* **UTF-8 and Unicode Support** - Trex supports UTF-8 and Unicode text.
* **SDF rendering** - Trex can render glyphs using the Signed Distance Field technique.
* **Subpixel Rendering (ClearType)** - Trex supports subpixel rendering for RGB LCD displays.
* **Emojis ❤️** - Trex can render fonts with colors, like emojis 💪😎👍

## Basic Example

```cpp
Trex::Atlas atlas("arial.ttf", 32, Trex::Charset::Ascii());
atlas.SaveToFile("atlas.png");

Trex::TextShaper shaper(atlas);
Trex::ShapedGlyphs glyphs = shaper.ShapeAscii("Hello world!");

float cursorX = 100;
float cursorY = 100;
for (const Trex::ShapedGlyph& glyph : glyphs)
{
    float x = cursorX + glyph.xOffset + glyph.info.bearingX;
    float y = cursorY + glyph.yOffset - glyph.info.bearingY;

    int atlasGlyphX = glyph.info.x;
    int atlasGlyphY = glyph.info.y;
    int atlasGlyphWidth = glyph.info.width;
    int atlasGlyphHeight = glyph.info.height;

    // Draw a glyph at (x, y) by taking the atlas bitmap fragment
    // ... atlas.GetBitmap() ...

    cursorX += glyph.xAdvance;
    cursorY += glyph.yAdvance;
}

```

## Getting Started
To get started with Trex, follow the instructions below:

1. Clone the repository:
```
git clone https://github.com/KyrietS/trex.git
```
2. In your project's CMakeLists.txt add:
```
add_subdirectory(path_to_trex)
target_link_libraries(your_target trex)
```
3. Rebuild your project.
4. Check the [examples/](examples/) to learn how to use Trex or go to the [docs/](docs/) to learn about Trex API.

Note: You can also use CMake's `FetchContent` module to download and configure Trex automatically.

```cmake
include(FetchContent)
FetchContent_Declare(
    trex
    GIT_REPOSITORY https://github.com/KyrietS/trex.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(trex)
target_link_libraries(your_target trex)
```

## Showcase

The text produced by trex has exactly the same size as the text rendered by Google Chrome and Firefox.

![trex-vs-chrome](https://github.com/KyrietS/trex/assets/19170699/05a332bd-72b9-4575-957e-9bacf1b3b65d)

_This is the reason why I created trex. I needed "Save to SVG" option in my application so I had to make sure that the text rendered by the application will perfectly match the SVG exported image._

Emoji support

![emojis](https://github.com/KyrietS/trex/assets/19170699/c5b42d59-2a22-4b5b-9785-29256e3babf1)

[Roboto](https://fonts.google.com/specimen/Roboto) ASCII grayscale (512 x 512)

![ASCII atlas](https://github.com/KyrietS/trex/assets/19170699/7780d0c5-259f-45db-a019-ad4388b2489b)


[Roboto](https://fonts.google.com/specimen/Roboto) all glyphs grayscale (2048 x 1024)

![Roboto atlas](https://github.com/KyrietS/trex/assets/19170699/8dda53ff-9c58-4dcb-b565-5d5b55d6b431)

[OpenMoji](https://github.com/hfg-gmuend/openmoji) all emojis

![open-moji](https://github.com/KyrietS/trex/assets/19170699/09d989b1-1ad1-4ced-bdaa-c138689029ce)


## Documentation
The documentation for the Trex API can be found [here](docs/README.md).

## Dependencies

Trex has the following dependencies:

* [FreeType](https://github.com/freetype/freetype) - A high-quality font engine for rendering text.
* [HarfBuzz](https://github.com/harfbuzz/harfbuzz) - A text shaping engine for accurate and complex text shaping.
* [stb_image_write](https://github.com/nothings/stb) - A header-only library for saving atlas bitmaps to PNG or BMP files.

Examples use [raylib](https://github.com/raysan5/raylib) library to render text on the screen.\
Tests use [Google Test](https://github.com/google/googletest) framework.

**All dependencies are fetched and configured automatically by CMake.**

## License
Copyright © 2023-2025 KyrietS\
Use of this software is granted under the terms of the MIT License.

See the [LICENSE](LICENSE) file for more details.
