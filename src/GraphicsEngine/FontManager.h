#pragma once

#include <map>
#include "TextLib/cText.h"

namespace ShiftEngine
{
	class FontManager
	{
	public:
		FontManager();

		void Shutdown();
		void DrawTextTL(const std::string & Text, int x, int y);
		void DrawTextTL(const std::string & Text, int x, int y, const ShiftEngine::gRect & rect);

		int GetFontHeight();
		int GetStringWidth(const std::string & string);

		void SetFont(const std::wstring & fontName);
		std::wstring GetCurrentFont() const;

	private:
		void LoadFonts();
		std::map<std::wstring, cFont *> Fonts;

		std::wstring currentFont;

		IProgramPtr TextShader;
	};
}
