#pragma once

#include "ShiftEngine.h"
#include "FontManager.h"
#include "../Utilities/ut.h"

ShiftEngine::FontManager::FontManager()
	: currentFont(L"")
	, pCurrentFont(nullptr)
	, TextShader(nullptr)
{
	D3D10ContextManager* pCntMng = ShiftEngine::GetContextManager();
	TextShader = pCntMng->LoadShader(L"text.fx");
	LoadFonts();
	if (!Fonts.empty())
		currentFont = Fonts.begin()->first;
}

void ShiftEngine::FontManager::Shutdown()
{
}

void ShiftEngine::FontManager::DrawTextTL(const std::string & Text, int x, int y)
{
	D3D10ContextManager* pCntMng = ShiftEngine::GetContextManager();
	ID3D10Device *pContext = pCntMng->GetDevicePointer();

	if (!pCurrentFont || !Text.size())
		return;

	auto cbs = pCntMng->GetBlendingState();
	pCntMng->SetBlendingState(BS_AlphaEnabled);
	auto crs = pCntMng->GetRasterizerState();
	pCntMng->SetRasterizerState(RS_Normal);

	cText tex(Text, pCurrentFont, pCntMng->GetDevicePointer());

	D3DXMATRIX mat, matOrtho, matT;
	D3DXMatrixTranslation(&matT, (float)x, (float)y, 0.0f);
	GraphicEngineSettings pSettings = pCntMng->GetParameters();
	D3DXMatrixOrthoOffCenterLH(&matOrtho, 0.0f, (float)pSettings.screenWidth, (float)pSettings.screenHeight, 0.0f, 0.0f, 1.0f);
	mat = matT * matOrtho;
	TextShader->SetMatrixConstantByName("matOrtho", (float*)mat);

	D3DXVECTOR4 vec;
	vec.x = 0.0f;
	vec.y = 0.0f;
	vec.z = 16000.0f;
	vec.w = 16000.0f;

	TextShader->SetVectorConstantByName("Rect", (float*)vec);
	TextShader->SetTextureByName("FontTexture", pCurrentFont->GetTexturePtr());
	TextShader->Apply(true);

	tex.Draw(pCntMng->GetDevicePointer());
	
	pCntMng->SetRasterizerState(crs);
	pCntMng->SetBlendingState(cbs);

	//for fix a lot of DXDEBUG warnings
	pCntMng->ResetPipeline();
}

void ShiftEngine::FontManager::DrawTextTL(const std::string & Text, int x, int y, const ShiftEngine::gRect & rect)
{
	D3D10ContextManager* pCntMng = ShiftEngine::GetContextManager();

	if (!pCurrentFont || !Text.size())
		return;
	
	auto crs = pCntMng->GetRasterizerState();
	pCntMng->SetRasterizerState(RS_Normal);

	cText tex(Text, pCurrentFont, pCntMng->GetDevicePointer());

	D3DXMATRIX mat, matOrtho, matT;
	D3DXMatrixTranslation(&matT, (float)x, (float)y, 0.0f);
	GraphicEngineSettings pSettings = pCntMng->GetParameters();
	D3DXMatrixOrthoOffCenterLH(&matOrtho, 0.0f, (float)pSettings.screenWidth, (float)pSettings.screenHeight, 0.0f, 0.0f, 1.0f);
	mat = matT * matOrtho;
	TextShader->SetMatrixConstantByName("matOrtho", (float*)mat);

	D3DXVECTOR4 vec;
	vec.x = rect.left;
	vec.y = rect.top;
	vec.z = rect.right;
	vec.w = rect.bottom;

	TextShader->SetVectorConstantByName("Rect", (float*)vec);
	TextShader->SetTextureByName("FontTexture", pCurrentFont->GetTexturePtr());
	TextShader->Apply(true);
	tex.Draw(pCntMng->GetDevicePointer());

	pCntMng->SetRasterizerState(crs);

	//for fix a lot of DXDEBUG warnings
	pCntMng->ResetPipeline();
}

int ShiftEngine::FontManager::GetFontHeight()
{
	if(!pCurrentFont)
		return 0;
	else
		return pCurrentFont->LineHeight;
}

int ShiftEngine::FontManager::GetStringWidth( const std::string & string )
{
	int result = 0;

	if (!pCurrentFont)
		return 0;

	for (auto iter = string.cbegin(); iter != string.cend(); iter++)
	{
		auto cp = pCurrentFont->GetCharacterPtr((*iter));
		if(!cp->Height)
			return 0;

		result += cp->XAdvance;
	}

	return result;
}

void ShiftEngine::FontManager::LoadFonts()
{
	D3D10ContextManager* pCntMng = ShiftEngine::GetContextManager();
	//TODO: добавить механизм, чтобы файл шрифта содержал имя текстуры
	PathSettings pPaths = pCntMng->GetPaths();
	auto fontsNames = utils::filesystem::CollectFileNames(pPaths.FontsPath, L"fnt2");
	auto fontsTextureNames = utils::filesystem::CollectFileNames(pPaths.TexturePath + L"fonts\\");

	if(fontsNames.size() == 0 || fontsTextureNames.size() == 0 || fontsNames.size() != fontsTextureNames.size())
		MainLog.FatalError("Unable to load textures for fonts");

	auto fontNamesIterator = fontsNames.begin();
	auto textureNamesIterator = fontsTextureNames.begin();

	while(fontNamesIterator != fontsNames.end())
	{
		cFont * font = new cFont;
		if(!font->Initialize(pPaths.FontsPath + *fontNamesIterator, pCntMng->LoadTexture(L"fonts\\" + *textureNamesIterator)))
			MainLog.Error("Unable to load font: " + utils::WStrToStr(*fontNamesIterator));
		Fonts[::utils::ExtractName(*fontNamesIterator)].reset(font);
		++textureNamesIterator;
		++fontNamesIterator;
	}
}

void ShiftEngine::FontManager::SetFont( const std::wstring & fontName )
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
		MainLog.Error("Unable to find font: " + utils::WStrToStr(fontName));
	}
}

std::wstring ShiftEngine::FontManager::GetCurrentFontName() const
{
	return currentFont;
}
