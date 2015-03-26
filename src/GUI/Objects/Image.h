#pragma once

#include "Base.h"
#include <GraphicsEngine\Sprite.h>

namespace SimpleGUI
{
    class Image : public Base
    {
    public:
        Image(Base * parent, const std::wstring & str);
        virtual ~Image();

        virtual void Draw(Skinner * skin) override;

        void SetImage(const std::wstring & imageName);

        ShiftEngine::Sprite & GetImage();
        const ShiftEngine::Sprite & GetImage() const;

    private:
        ShiftEngine::Sprite image;
    };

    typedef std::shared_ptr<Image> pImage;
}
