#include "Trex/Atlas.hpp"

int main()
{
	constexpr int FONT_SIZE = 64;
	const char* FONT_PATH = "fonts/Roboto-Regular.ttf";

	Trex::Atlas atlas(FONT_PATH, FONT_SIZE, Trex::Charset::Ascii());
	atlas.SaveToFile("Example_1_Atlas_ASCII.png");

	return 0;
}
