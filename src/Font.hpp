#pragma once
#include <cstdint>

struct FT_FaceRec_;

namespace Trex
{
	struct Pixels { int value; };
	struct Points { int value; };

	class Font
	{
	public:
		explicit Font(const char* path);
		Font(const Font&);
		Font(Font&&) noexcept;
		~Font();

		void SetSize(Pixels size);
		void SetSize(Points size);
		uint32_t GetGlyphIndex(uint32_t codepoint) const;

		Font& operator=(const Font&) noexcept;
		Font& operator=(Font&&) noexcept;

		FT_FaceRec_* face = nullptr;
	};
}