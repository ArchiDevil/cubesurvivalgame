#include "FontManager.h"

#include "ShiftEngine.h"

#include <Utilities/ut.h>

#include <cassert>

ShiftEngine::FontManager::FontManager()
    : currentFont(L"")
    , textShader(nullptr)
{
    D3D10ContextManager* pCntMng = GetContextManager();
    textShader = pCntMng->LoadShader(L"text.fx");
    LoadFonts();
    if (!Fonts.empty())
        currentFont = Fonts.begin()->first;
}

void ShiftEngine::FontManager::DrawTextTL(const std::string & Text, float x, float y)
{
    if (!pCurrentFont || !Text.size())
        return;

    BatchText(Text, x, y);
}

int ShiftEngine::FontManager::GetFontHeight()
{
    if (!pCurrentFont)
        return 0;
    else
        return pCurrentFont->LineHeight;
}

int ShiftEngine::FontManager::GetStringWidth(const std::string & string)
{
    int result = 0;

    if (!pCurrentFont)
        return 0;

    for (auto iter = string.cbegin(); iter != string.cend(); iter++)
    {
        auto cp = pCurrentFont->GetCharacterPtr((*iter));
        if (!cp->Height)
            return 0;

        result += cp->XAdvance;
    }

    return result;
}

void ShiftEngine::FontManager::LoadFonts()
{
    D3D10ContextManager* pCntMng = GetContextManager();
    //TODO: добавить механизм, чтобы файл шрифта содержал имя текстуры
    PathSettings pPaths = pCntMng->GetPaths();
    auto fontsNames = utils::filesystem::CollectFileNames(pPaths.FontsPath, L"fnt2");
    auto fontsTextureNames = utils::filesystem::CollectFileNames(pPaths.TexturePath + L"fonts\\");

    if (fontsNames.size() == 0 || fontsTextureNames.size() == 0 || fontsNames.size() != fontsTextureNames.size())
        LOG_FATAL_ERROR("Unable to load textures for fonts");

    auto fontNamesIterator = fontsNames.begin();
    auto textureNamesIterator = fontsTextureNames.begin();

    while (fontNamesIterator != fontsNames.end())
    {
        cFont * font = new cFont;
        if (!font->Initialize(pPaths.FontsPath + *fontNamesIterator, pCntMng->LoadTexture(L"fonts\\" + *textureNamesIterator)))
            LOG_ERROR("Unable to load font: ", utils::Narrow(*fontNamesIterator));
        Fonts[::utils::ExtractName(*fontNamesIterator)].reset(font);
        ++textureNamesIterator;
        ++fontNamesIterator;
    }
}

void ShiftEngine::FontManager::SetFont(const std::wstring & fontName)
{
    auto iter = Fonts.find(fontName);
    if (iter != Fonts.end())
    {
        currentFont = fontName;
        pCurrentFont = iter->second.get();
    }
    else
    {
        pCurrentFont = nullptr;
        currentFont.clear();
        LOG_ERROR("Unable to find font: ", utils::Narrow(fontName));
    }
}

std::wstring ShiftEngine::FontManager::GetCurrentFontName() const
{
    return currentFont;
}

void ShiftEngine::FontManager::BatchText(const std::string & text, float x, float y)
{
    if (text.size() == 0)
        return;

    auto & v = batchedVertices[GetCurrentFontName()];
    auto & i = batchedIndices[GetCurrentFontName()];

    size_t vertSh = v.size();
    size_t indSh = i.size();

    i.resize(i.size() + text.size() * 6);
    v.resize(v.size() + text.size() * 4);

    int CurX = 0;

    for (char symbol : text)
    {
        sChar * cp = pCurrentFont->GetCharacterPtr(symbol);
        assert(cp->Height != 0);
        int CharX = cp->x;
        int CharY = cp->y;
        int Width = cp->Width;
        int Height = cp->Height;
        int OffsetX = cp->XOffset;
        int OffsetY = cp->YOffset;

        //first triangle
        i[indSh++] = vertSh;
        i[indSh++] = vertSh + 1;
        i[indSh++] = vertSh + 2;

        //second triangle
        i[indSh++] = vertSh;
        i[indSh++] = vertSh + 2;
        i[indSh++] = vertSh + 3;

        /*
        0   1
        -----
        |\  |
        | \ |
        |  \|
        -----
        3   2
        */

        //upper left
        v[vertSh].tu = (float)CharX / (float)pCurrentFont->Width;
        v[vertSh].tv = (float)CharY / (float)pCurrentFont->Height;
        v[vertSh].x = (float)CurX + OffsetX + x;
        v[vertSh++].y = (float)OffsetY + y;

        //upper right
        v[vertSh].tu = (float)(CharX + Width) / (float)pCurrentFont->Width;
        v[vertSh].tv = (float)CharY / (float)pCurrentFont->Height;
        v[vertSh].x = (float)Width + CurX + OffsetX + x;
        v[vertSh++].y = (float)OffsetY + y;

        //lower right
        v[vertSh].tu = (float)(CharX + Width) / (float)pCurrentFont->Width;
        v[vertSh].tv = (float)(CharY + Height) / (float)pCurrentFont->Height;
        v[vertSh].x = (float)Width + CurX + OffsetX + x;
        v[vertSh++].y = (float)Height + OffsetY + y;

        //lower left
        v[vertSh].tu = (float)CharX / (float)pCurrentFont->Width;
        v[vertSh].tv = (float)(CharY + Height) / (float)pCurrentFont->Height;
        v[vertSh].x = (float)CurX + OffsetX + x;
        v[vertSh++].y = (float)Height + OffsetY + y;

        CurX += cp->XAdvance;
    }
}

void ShiftEngine::FontManager::DrawBatchedText()
{
    for (auto & p : batchedVertices)
    {
        auto & v = batchedVertices[p.first];
        auto & i = batchedIndices[p.first];

        if (v.empty() || i.empty())
            return;

        auto* pCtxMgr = GetContextManager();

        batchedMesh = pCtxMgr->GetMeshManager()->CreateMeshFromVertices((uint8_t*)v.data(), v.size() * sizeof(TextPoint), i, &plainSpriteVertexSemantic);

        auto cbs = pCtxMgr->GetBlendingState();
        pCtxMgr->SetBlendingState(BS_AlphaEnabled);
        auto crs = pCtxMgr->GetRasterizerState();
        pCtxMgr->SetRasterizerState(RS_Normal);

        D3DXMATRIX matOrtho;
        GraphicEngineSettings pSettings = pCtxMgr->GetEngineSettings();
        D3DXMatrixOrthoOffCenterLH(&matOrtho, 0.0f, (float)pSettings.screenWidth, (float)pSettings.screenHeight, 0.0f, 0.0f, 1.0f);
        textShader->SetMatrixConstantByName("matOrtho", (float*)matOrtho);

        D3DXVECTOR4 vec;
        vec.x = 0.0f;
        vec.y = 0.0f;
        vec.z = 16000.0f;
        vec.w = 16000.0f;

        textShader->SetVectorConstantByName("Rect", (float*)vec);
        SetFont(p.first);
        textShader->SetTextureByName("FontTexture", pCurrentFont->GetTexturePtr());
        textShader->Apply(true);

        batchedMesh->GetVertexDeclaration()->Bind();
        batchedMesh->Draw();

        pCtxMgr->SetRasterizerState(crs);
        pCtxMgr->SetBlendingState(cbs);

        //for fix a lot of DXDEBUG warnings
        pCtxMgr->ResetPipeline();

        v.clear();
        i.clear();
        batchedMesh->Clear();
    }
}
