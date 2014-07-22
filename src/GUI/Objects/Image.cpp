#include "Image.h"

namespace SimpleGUI
{
	Image::Image( Base * parent, const std::wstring & str )
		: Base(parent), image(str) {}

	Image::~Image() {}

	void Image::Draw( Skinner * skin )
	{
		skin->DrawImage(this, image);
		this->RecursiveDrawing(skin);
	}

	void Image::SetImage( const std::wstring & image )
	{
		return;
		//this->image.
		//imageName = image;
	}

}
