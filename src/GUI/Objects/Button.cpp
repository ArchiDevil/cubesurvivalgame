#include "Button.h"

SimpleGUI::Button::Button( Base * parent )
	: Base(parent), Selected(false), img(nullptr),
	clickHandler(nullptr)
{}

SimpleGUI::Button::~Button()
{}

void SimpleGUI::Button::Draw( Skinner * skin )
{
	if(!skin)
		return;

	skin->DrawButton(this);

	if(this->img)
		img->Render(skin);
}

bool SimpleGUI::Button::OnMouseEnter()
{
	Selected = true;
	return true;
}

bool SimpleGUI::Button::OnMouseLeave()
{
	Selected = false;
	return true;
}

bool SimpleGUI::Button::IsSelected() const
{
	return Selected;
}

std::string SimpleGUI::Button::GetText() const
{
	return text;
}

void SimpleGUI::Button::SetText( const std::string & _text )
{
	text = _text;
}

bool SimpleGUI::Button::OnMouseUp( int mb, int innerX, int innerY )
{
	if((bool)clickHandler == true)
		clickHandler(mb, innerX, innerY);
	return true;
}

void SimpleGUI::Button::SetPicture( const std::wstring & str, bool center )
{
	//TODO: create alignment to the: center / left-top corner
	textureName = str;

	//if(this->img)
	//	delete img;

	img = std::make_shared<Image>(SimpleGUI::Image(nullptr, textureName));
	Point size = GetSize();
	size.x = (int)((float)size.x / 1.2f);
	size.y = (int)((float)size.y / 1.2f);
	img->SetSize(size.x, size.y);
	Point curButSize = this->GetSize();
	curButSize = Point(curButSize.x / 2, curButSize.y / 2);
	img->SetPosition(Point(this->GetPosition().x + curButSize.x - img->GetSize().x / 2,
		this->GetPosition().y + curButSize.y - img->GetSize().y / 2));
}

void SimpleGUI::Button::SetClickHandler( std::function <void(int mb, int innerX, int innerY)> _handler )
{
	clickHandler = _handler;
}