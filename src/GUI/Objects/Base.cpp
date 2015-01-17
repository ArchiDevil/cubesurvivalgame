#include "Base.h"

namespace SimpleGUI
{
	Base::Base( Base * parent, std::string name /*= ""*/ ) 
		: Position(0, 0), visible(true), 
		Parent(parent), Size(1, 1),	Name(name)
	{
		if(Parent)
			Parent->AddChildren((pBase)this);
	}

	Base::~Base()
	{
		if(SimpleGUI::HoveredControl == this)
			SimpleGUI::HoveredControl = nullptr;
		if(SimpleGUI::FocusedControl == this)
			SimpleGUI::FocusedControl = nullptr;
	}

	Point Base::GetPosition() const
	{
		if(Parent)
			return (Parent->GetPosition() + Position);
		else
			return Position;
	}

	void Base::SetPosition( Point Pos )	{Position = Pos;}
	void Base::SetPosition( int x, int y ) {Position = Point(x, y);}

	bool Base::IsVisible() const {return visible;}

	bool Base::IsHit( const Point & point )
	{
		//			   TOP
		//			---------
		//			|		|
		//	LEFT	|		|	RIGHT
		//			|		|
		//			---------
		//			  BOTTOM
		Point realPos = Position;

		if(Parent)
			realPos += Parent->GetPosition();

		if(point.x < realPos.x)
			return false;

		if(point.y < realPos.y)
			return false;

		if(point.x > realPos.x + Size.x)
			return false;

		if(point.y > realPos.y + Size.y)
			return false;

		return true;
	}

	bool Base::OnMouseMove(){return false;}
	bool Base::OnMouseEnter(){return false;}
	bool Base::OnMouseLeave(){return false;}
	bool Base::OnMouseDown( int /*mb*/, int /*x*/, int /*y*/ ){return false;}
	bool Base::OnMouseUp( int /*mb*/, int /*x*/, int /*y*/ ){return false;}

	bool Base::OnKeyChar( wchar_t /*Key*/ ){return false;}
	bool Base::OnKeyUp( wchar_t /*Key*/ ){return false;}
	bool Base::OnKeyDown( wchar_t /*Key*/ ){return false;}

	bool Base::CanHaveFocus() const {return false;}

	void Base::Show()
	{
		visible = true;
	}

	void Base::Hide()
	{
		visible = false;
	}

	void Base::AddChildren( pBase Child )
	{
		if(!Child)
			return;

		Children.push_back(Child);

		if(Child->Parent && Child->Parent != this)
			Child->Parent->RemoveChildren(Child);

		Child->Parent = this;
	}

	void Base::RemoveChildren( pBase Child )
	{
		if(!Child)
			return;

		Children.erase( std::find(Children.begin(), Children.end(), Child) );
	}

	void Base::RemoveAllChildrens()
	{
		SimpleGUI::HoveredControl = nullptr;	//remove hovered
		Children.clear();
	}

	SimpleGUI::Point Base::GetSize() const {return Size;}
	void Base::SetSize( int x, int y ){Size = Point(x, y);}

	Base * Base::GetControlAt( int mouseX, int mouseY )
	{
		for (auto iter = Children.begin(); iter != Children.end(); iter++)
		{
			Base * elem = &*(*iter);
			if( elem->IsHit(Point(mouseX, mouseY)) )
			{
				return elem->GetControlAt(mouseX, mouseY);
			}
		}
		return this;
	}

	void Base::Draw( Skinner * skin )
	{
		if(!skin)
			return;

		RecursiveDrawing(skin);
	}

	void Base::RecursiveDrawing( Skinner * skin )
	{
		if(!skin)
			return;		//unreachable code for now, but what if i'll change code later?

		for (auto iter = Children.begin(); iter != Children.end(); iter++)
			(*iter)->Render(skin);
	}

	void Base::Render(Skinner * skin)
	{
		if(this->IsVisible())
		{
			this->Draw(skin);
			RecursiveDrawing(skin);
		}
	}

	Base * Base::GetParent() const
	{
		return Parent;
	}

	std::string Base::GetName() const
	{
		return Name;
	}

	void Base::SetName( const std::string & val )
	{
		Name = val;
	}

	const std::list<pBase> Base::GetChildrenList() const
	{
		return Children;
	}

}