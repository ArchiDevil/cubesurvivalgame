#pragma once

#include "TextLib/cText.h"

#include "IProgram.h"

#include <map>

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
        std::wstring GetCurrentFontName() const;

    private:
        void LoadFonts();
        std::map<std::wstring, std::unique_ptr<cFont>> Fonts;
        cFont * pCurrentFont = nullptr;
        std::wstring currentFont;
        IProgramPtr TextShader;
    };
}
