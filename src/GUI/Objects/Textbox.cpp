#include "Textbox.h"

SimpleGUI::Textbox::Textbox(Base * _parent)
    : Base(_parent), currentText(L"")
{}

SimpleGUI::Textbox::~Textbox() {}

void SimpleGUI::Textbox::Draw(Skinner * skin)
{
    if (!skin)
        return;

    skin->DrawTextbox(this, utils::WStrToStr(currentText), SimpleGUI::FocusedControl == this);
}

bool SimpleGUI::Textbox::CanHaveFocus() const
{
    return true;
}

bool SimpleGUI::Textbox::OnKeyChar(wchar_t Key)
{
    if (Key == '\b')
    {
        if (currentText.size() != 0)
        {
            currentText.pop_back();
            OnTextChanged();
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (isalpha(Key) || isdigit(Key))
    {
        currentText.push_back(Key);
        OnTextChanged();
        return true;
    }
    else
    {
        return false;
    }
}

std::wstring SimpleGUI::Textbox::GetText() const
{
    return currentText;
}

void SimpleGUI::Textbox::SetOnTextChangedCallback(std::function<void(const std::wstring & newText)> textChanged)
{
    textChangedHandler = textChanged;
}

void SimpleGUI::Textbox::OnTextChanged()
{
    if (textChangedHandler)
        textChangedHandler(this->currentText);
}

void SimpleGUI::Textbox::SetText(const std::wstring & text)
{
    currentText = text;
    OnTextChanged();
}
