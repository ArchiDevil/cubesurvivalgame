#pragma once

#include "Base.h"
#include <GraphicsEngine\Sprite.h>

//Image contains some picture sprite and draw it at the screen, so =)

namespace SimpleGUI
{
    class Image : public Base
    {
    public:
        Image(Base * parent, const std::wstring & str);
        virtual ~Image();

        virtual void Draw(Skinner * skin) override;
        void SetImage(const std::wstring & imageName);

    private:
        ShiftEngine::Sprite image;
    };

    typedef std::shared_ptr<Image> pImage;
}
