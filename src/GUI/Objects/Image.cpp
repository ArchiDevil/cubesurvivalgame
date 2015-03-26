#include "Image.h"

SimpleGUI::Image::Image(Base * parent, const std::wstring & str)
    : Base(parent)
    , image(str)
{
}

SimpleGUI::Image::~Image()
{
}

void SimpleGUI::Image::Draw(Skinner * skin)
{
    skin->DrawImage(this);
    this->RecursiveDrawing(skin);
}

void SimpleGUI::Image::SetImage(const std::wstring & imageName)
{
    image.SetTexture(imageName);
}

ShiftEngine::Sprite & SimpleGUI::Image::GetImage()
{
    return image;
}

const ShiftEngine::Sprite & SimpleGUI::Image::GetImage() const
{
    return image;
}
