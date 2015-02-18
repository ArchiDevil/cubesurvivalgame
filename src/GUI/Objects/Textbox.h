#pragma once

#include <functional>

#include "Text.h"

namespace SimpleGUI
{
    class Textbox : public Base
    {
    public:
        Textbox(Base * _parent);
        ~Textbox();

        virtual void Draw(Skinner * skin) override;
        virtual bool CanHaveFocus() const override;
        virtual bool OnKeyChar(wchar_t Key) override;

        std::wstring GetText() const;
        void SetText(const std::wstring & text);

        void OnTextChanged();
        void SetOnTextChangedCallback(std::function<void(const std::wstring & newText)> textChanged);

    private:
        std::function<void(const std::wstring & newText)> textChangedHandler;
        std::wstring currentText;
    };
}
