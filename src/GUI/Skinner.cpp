#include "Skinner.h"

#include "Objects/Button.h"
#include "Objects/Textbox.h"
#include "Objects/Image.h"
#include "Objects/List.h"
#include "Objects/Table.h"
#include "Objects/Text.h"
#include "Objects/ProgressBar.h"
#include "Objects/ValueBox.h"
#include "Objects/Window.h"

SimpleGUI::Skinner::Skinner()
    : TextboxCache(nullptr)
    , ListCache(nullptr)
    , boxCache(nullptr)
{
}

SimpleGUI::Skinner::~Skinner()
{
}

void SimpleGUI::Skinner::Initialize()
{
    LoadResources();
}

void SimpleGUI::Skinner::LoadResources()
{
    TextboxCache.reset(new ShiftEngine::Sprite(L"gui/textbox.png"));
    ListCache.reset(new ShiftEngine::Sprite(L"gui/list.png"));
    boxCache.reset(new ShiftEngine::Sprite(L"gui/box.png"));
}

void SimpleGUI::Skinner::DrawButton(Button * but)
{
    ShiftEngine::D3D10ContextManager * cm = ShiftEngine::GetContextManager();

    Vector4F maskColor = {};
    if (but->IsSelected())
        maskColor = { 1.0f, 0.8f, 0.8f, 1.0f };
    else
        maskColor = { 1.0f, 1.0f, 1.0f, 1.0f };

    auto pos = but->GetPosition();
    auto size = but->GetSize();
    SetControlParameters(boxCache.get(), pos, size);
    boxCache->SetMaskColor(Vector4F(1.0f * maskColor.x, 1.0f * maskColor.y, 1.0f * maskColor.z, 1.0f * maskColor.w));
    boxCache->Draw();

    pos = pos + Point(2, 2);
    size = size - Point(4, 4);
    SetControlParameters(boxCache.get(), pos, size);
    boxCache->SetMaskColor(Vector4F(0.7f * maskColor.x, 0.7f * maskColor.y, 0.7f * maskColor.z, 1.0f * maskColor.w));
    boxCache->Draw();

    const std::string text = but->GetText();
    if (!text.empty())
    {
        ShiftEngine::FontManager * pFntMgr = cm->GetFontManager();
        Point textPos;

        auto prevFont = pFntMgr->GetCurrentFontName();
        pFntMgr->SetFont(L"guiDefault");

        textPos.x = pos.x + (size.x / 2) - pFntMgr->GetStringWidth(text) / 2;
        textPos.y = pos.y + (size.y / 2) - (pFntMgr->GetFontHeight() / 2);

        ShiftEngine::gRect rect;
        rect.left = (float)pos.x + 1;
        rect.right = (float)pos.x + size.x - 1;
        rect.top = (float)pos.y + 1;
        rect.bottom = (float)pos.y + size.y - 1;
        pFntMgr->DrawTextTL(text, textPos.x, textPos.y, rect);
        pFntMgr->SetFont(prevFont);
    }
}

void SimpleGUI::Skinner::DrawTextbox(TextBox * textbox, bool Focused)
{
    ShiftEngine::D3D10ContextManager * cm = ShiftEngine::GetContextManager();

    if (Focused)
        TextboxCache->SetMaskColor(Vector4F(1.0f, 0.8f, 0.8f, 1.0f));
    else
        TextboxCache->SetMaskColor(Vector4F(1.0f, 1.0f, 1.0f, 1.0f));

    auto pos = textbox->GetPosition();
    auto size = textbox->GetSize();
    SetControlParameters(TextboxCache.get(), pos, size);

    TextboxCache->Draw();

    std::string string = utils::WStrToStr(textbox->GetText());

    if (string.empty())
        return;

    Point textPosition;

    ShiftEngine::FontManager * pFntMgr = cm->GetFontManager();

    auto prevFont = pFntMgr->GetCurrentFontName();
    pFntMgr->SetFont(L"guiDefault");

    int stringWidth = pFntMgr->GetStringWidth(string);
    if (stringWidth > (size.x - size.x * 0.1f))	//more than size - 10%
        textPosition.x = pos.x + (size.x - (int)(size.x * 0.1f)) - stringWidth;
    else
        textPosition.x = pos.x + 3;

    textPosition.y = pos.y + (size.y / 2) - (pFntMgr->GetFontHeight() / 2);

    ShiftEngine::gRect rect;
    rect.left = (float)pos.x + 1;
    rect.right = (float)pos.x + size.x - 1;
    rect.top = (float)pos.y + 1;
    rect.bottom = (float)pos.y + size.y - 1;
    pFntMgr->DrawTextTL(string, textPosition.x, textPosition.y, rect);
    pFntMgr->SetFont(prevFont);
}

void SimpleGUI::Skinner::DrawImage(Image * image, const ShiftEngine::Sprite & spr)
{
    ShiftEngine::Sprite imageCache = spr;

    auto pos = image->GetPosition();
    auto size = image->GetSize();
    SetControlParameters(&imageCache, pos, size);

    imageCache.Draw();
}

void SimpleGUI::Skinner::DrawList(List * list)
{
    auto pos = list->GetPosition();
    auto size = list->GetSize();
    SetControlParameters(ListCache.get(), pos, size);

    ListCache->Draw();
}

void SimpleGUI::Skinner::DrawListRow(TableRow * row)
{
    ShiftEngine::D3D10ContextManager * cm = ShiftEngine::GetContextManager();

    std::string str = row->GetString();
    if (row->IsSelected())
        str = "-> " + str;

    ShiftEngine::FontManager * pFntMgr = cm->GetFontManager();
    auto prevFont = pFntMgr->GetCurrentFontName();
    pFntMgr->SetFont(L"guiDefault");
    ShiftEngine::gRect rect;
    rect.bottom = (float)row->GetPosition().y + (float)row->GetSize().y;
    rect.left = (float)row->GetPosition().x;
    rect.right = (float)row->GetPosition().x + (float)row->GetSize().x;
    rect.top = (float)row->GetPosition().y;
    pFntMgr->DrawTextTL(str, row->GetPosition().x + 1, row->GetPosition().y + 2, rect);
    pFntMgr->SetFont(prevFont);
}

void SimpleGUI::Skinner::SetControlParameters(ShiftEngine::Sprite * spr, const Point & pos, const Point & size)
{
    spr->SetPosition(Vector2F((float)pos.x + size.x / 2, (float)pos.y + size.y / 2));
    FloatSize scale = CalculateScaling(size.x, size.y, (int)spr->GetTextureDimensions().x, (int)spr->GetTextureDimensions().y);
    spr->SetScale(Vector2F(scale.x, scale.y));
}

void SimpleGUI::Skinner::DrawLabel(Text * label)
{
    ShiftEngine::D3D10ContextManager * cm = ShiftEngine::GetContextManager();

    if (label->GetText().empty())
        return;

    Point textPosition = label->GetPosition();

    ShiftEngine::FontManager * pFntMgr = cm->GetFontManager();
    auto prevFont = pFntMgr->GetCurrentFontName();
    pFntMgr->SetFont(L"guiDefault");
    pFntMgr->DrawTextTL(label->GetText(), textPosition.x, textPosition.y);
    pFntMgr->SetFont(prevFont);
}

void SimpleGUI::Skinner::DrawValueBox(ValueBox * valueBox)
{
    const float marginSize = 0.2f;

    //draw outer border first
    boxCache->SetMaskColor(Vector4F(0.25f, 0.25f, 0.25f, 1.0f));
    const auto outerPos = valueBox->GetPosition();
    const auto outerSize = valueBox->GetSize();
    SetControlParameters(boxCache.get(), outerPos, outerSize);
    boxCache->Draw();

    //draw inner border
    boxCache->SetMaskColor(Vector4F(0.75f, 0.75f, 0.75f, 1.0f));
    auto innerPos = outerPos;
    auto innerSize = outerSize;
    int minSize = (int)((float)min(innerSize.x, innerSize.y) * marginSize);
    innerSize.x -= minSize;
    innerSize.y -= minSize;
    innerPos.x += minSize / 2;
    innerPos.y += minSize / 2;
    SetControlParameters(boxCache.get(), innerPos, innerSize);
    boxCache->Draw();

    const int delimWidth = (int)((float)outerSize.x * 0.08f);

    //now need to draw value delimiter
    auto delimPos = outerPos;
    auto delimSize = outerSize;
    delimSize.x = delimWidth;
    delimSize.y = outerSize.y;
    float percent = (float)(valueBox->GetValue() - valueBox->GetMinValue()) / (valueBox->GetMaxValue() - valueBox->GetMinValue());
    delimPos.x = (int)MathLib::LinearInterpolation((float)outerPos.x, (float)outerPos.x + (float)outerSize.x, percent) - delimWidth / 2;
    delimPos.y = outerPos.y;
    SetControlParameters(boxCache.get(), delimPos, delimSize);
    boxCache->SetMaskColor(Vector4F(0.5f, 0.5f, 0.5f, 1.0f));
    boxCache->Draw();

    //drawing border of delimiter
    boxCache->SetMaskColor(Vector4F(1.0f, 1.0f, 1.0f, 1.0f));
    auto innerDelimPos = delimPos;
    auto innerDelimSize = delimSize;
    minSize = (int)((float)min(innerDelimSize.x, innerDelimSize.y) * marginSize);
    innerDelimSize.x -= minSize;
    innerDelimSize.y -= minSize;
    innerDelimPos.x += minSize / 2;
    innerDelimPos.y += minSize / 2;
    SetControlParameters(boxCache.get(), innerDelimPos, innerDelimSize);
    boxCache->Draw();
}

void SimpleGUI::Skinner::DrawWindow(Window * window)
{
    auto pos = window->GetPosition();
    auto size = window->GetSize();
    SetControlParameters(boxCache.get(), pos, size);
    boxCache->SetMaskColor(MathLib::Vector4F(1.0f, 1.0f, 1.0f, 1.0f));
    boxCache->Draw();

    size.x -= 4;
    size.y -= 4;
    boxCache->SetSizeInPixels(size.x, size.y);
    boxCache->SetMaskColor(MathLib::Vector4F(0.75f, 0.75f, 0.75f, 1.0f));
    boxCache->Draw();
}

void SimpleGUI::Skinner::DrawProgressBar(ProgressBar* pBar)
{
    throw std::exception("The method or operation is not implemented.");
}
