#include <fstream>
#include "Trex/Atlas.hpp"
#include "Trex/Font.hpp"

std::vector<uint8_t> LoadFontFileData(const char* path)
{
	std::ifstream fontFileStream(path, std::ios::binary);
	std::vector<uint8_t> fontFileData((std::istreambuf_iterator<char>(fontFileStream)),
									  std::istreambuf_iterator<char>());
	return fontFileData;
}

int main()
{
	constexpr int FONT_SIZE = 64;
	const char* FONT_PATH = "fonts/Roboto-Regular.ttf";

	// Get font file as a vector of bytes
	std::vector<uint8_t> fontFileData = LoadFontFileData(FONT_PATH);
	Trex::Atlas atlas(fontFileData, FONT_SIZE, Trex::Charset::Ascii());
	atlas.SaveToFile("Example_4_Atlas_FromMemory.png");

	return 0;
}
