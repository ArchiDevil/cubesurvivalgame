#include "TextBox.h"

SimpleGUI::TextBox::TextBox(Base * _parent)
    : Base(_parent)
    , currentText(L"")
{
}

SimpleGUI::TextBox::~TextBox() 
{
}

void SimpleGUI::TextBox::Draw(Skinner * skin)
{
    if (!skin)
        return;

    skin->DrawTextbox(this, SimpleGUI::FocusedControl == this);
}

bool SimpleGUI::TextBox::CanHaveFocus() const
{
    return true;
}

bool SimpleGUI::TextBox::OnKeyChar(wchar_t Key)
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

std::wstring SimpleGUI::TextBox::GetText() const
{
    return currentText;
}

void SimpleGUI::TextBox::SetOnTextChangedCallback(std::function<void(const std::wstring & newText)> textChanged)
{
    textChangedHandler = textChanged;
}

void SimpleGUI::TextBox::OnTextChanged()
{
    if (textChangedHandler)
        textChangedHandler(this->currentText);
}

void SimpleGUI::TextBox::SetText(const std::wstring & text)
{
    currentText = text;
    OnTextChanged();
}
