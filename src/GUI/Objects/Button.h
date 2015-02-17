#pragma once

#include "Base.h"
#include "Image.h"

#include <functional>

namespace SimpleGUI
{
    class Button : public Base
    {
    public:
        Button(Base * parent);
        ~Button();

        virtual void Draw(Skinner * skin) override;
        virtual bool OnMouseEnter() override;
        virtual bool OnMouseLeave() override;
        virtual bool OnMouseUp(MouseKeys mb, int innerX, int innerY) override;

        void SetPicture(const std::wstring & str, bool center);
        void SetClickHandler(std::function <void(MouseKeys mb, int innerX, int innerY)> _handler);
        bool IsSelected() const;

        std::string GetText() const;
        void SetText(const std::string & _text);

    private:
        std::function <void(MouseKeys mb, int innerX, int innerY)> clickHandler;
        pImage img;
        bool Selected;
        std::string text;
        std::wstring textureName;

    };
}
