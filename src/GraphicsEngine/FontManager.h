#pragma once

#include "TextLib/cFont.h"
#include "IProgram.h"
#include "IMeshData.h"

#include <map>
#include <vector>

namespace ShiftEngine
{
    class FontManager
    {
        struct TextPoint
        {
            float x, y, tu, tv;
        };

    public:
        FontManager();

        void DrawTextTL(const std::string & Text, float x, float y);

        int GetFontHeight();
        int GetStringWidth(const std::string & string);

        void SetFont(const std::wstring & fontName);
        std::wstring GetCurrentFontName() const;

        void DrawBatchedText();

    private:
        void LoadFonts();
        void BatchText(const std::string & text, float x, float y);

        std::vector<TextPoint> batchedVertices;
        std::vector<uint32_t> batchedIndices;

        std::map<std::wstring, std::unique_ptr<cFont>> Fonts;
        cFont * pCurrentFont = nullptr;
        std::wstring currentFont = L"";
        IProgramPtr textShader = nullptr;
        IMeshDataPtr batchedMesh = nullptr;
    };
}
