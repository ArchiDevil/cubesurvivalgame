#pragma once

#include <functional>

#include "Text.h"

namespace SimpleGUI
{
    class TextBox : public Base
    {
    public:
        TextBox(Base * _parent);
        ~TextBox();

        virtual void Draw(Skinner * skin) override;
        virtual bool CanHaveFocus() const override;
        virtual bool OnKeyChar(wchar_t Key) override;

        std::wstring GetText() const;
        void SetText(const std::wstring & text);

        void SetOnTextChangedCallback(std::function<void(const std::wstring & newText)> textChanged);

    protected:
        void OnTextChanged();

        std::function<void(const std::wstring & newText)> textChangedHandler;
        std::wstring currentText;
    };
}
