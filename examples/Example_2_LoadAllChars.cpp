#include "Trex/Atlas.hpp"

int main()
{
	constexpr int FONT_SIZE = 64;
	const char* FONT_PATH = "fonts/Roboto-Regular.ttf";

	Trex::Atlas atlas(FONT_PATH, FONT_SIZE, Trex::Charset::Full());
	atlas.SaveToFile("Example_2_Atlas_FULL.png");

	return 0;
}
