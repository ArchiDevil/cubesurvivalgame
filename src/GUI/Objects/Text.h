#pragma once

#include <string>

#include "Base.h"

namespace SimpleGUI
{
    class Text : public Base    //means LABEL
    {
    public:
        Text(Base * parent, const std::string & str)
            : Base(parent)
            , text(str)
        {
        }

        ~Text() 
        {
        }

        virtual void Draw(Skinner * skin) override
        {
            skin->DrawLabel(this);

            RecursiveDrawing(skin);
        }

        void SetText(const std::string & str)
        {
            text = str;
        }

        std::string GetText()
        {
            return text;
        }

    private:
        std::string text;

    };
}
