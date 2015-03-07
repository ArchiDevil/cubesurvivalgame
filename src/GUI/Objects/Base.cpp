#include "Base.h"

SimpleGUI::Base::Base(Base * parent, std::string name /*= ""*/)
    : Position(0, 0)
    , visible(true)
    , Parent(parent)
    , Size(1, 1)
    , Name(name)
{
    if (Parent)
    {
        // buggy!
        Parent->AddChildren(pBase(this));
    }
}

SimpleGUI::Base::~Base()
{
    if (SimpleGUI::HoveredControl == this)
        SimpleGUI::HoveredControl = nullptr;
    if (SimpleGUI::FocusedControl == this)
        SimpleGUI::FocusedControl = nullptr;
}

SimpleGUI::Point SimpleGUI::Base::GetPosition() const
{
    if (Parent)
        return (Parent->GetPosition() + Position);
    else
        return Position;
}

void SimpleGUI::Base::SetPosition(Point Pos)
{
    Position = Pos;
}

void SimpleGUI::Base::SetPosition(int x, int y)
{
    Position = Point(x, y);
}

bool SimpleGUI::Base::IsVisible() const
{
    return visible;
}

bool SimpleGUI::Base::IsHit(const Point & point)
{
    Point realPos = GetPosition();

    if (point.x < realPos.x)
        return false;

    if (point.y < realPos.y)
        return false;

    if (point.x > realPos.x + Size.x)
        return false;

    if (point.y > realPos.y + Size.y)
        return false;

    return true;
}

bool SimpleGUI::Base::CanHaveFocus() const
{
    return false;
}

void SimpleGUI::Base::Show()
{
    visible = true;
}

void SimpleGUI::Base::Hide()
{
    visible = false;
}

void SimpleGUI::Base::AddChildren(pBase Child)
{
    if (!Child)
        return;

    Children.push_back(Child);

    if (Child->Parent && Child->Parent != this)
        Child->Parent->RemoveChildren(Child);

    Child->Parent = this;
}

void SimpleGUI::Base::RemoveChildren(pBase Child)
{
    if (!Child)
        return;

    Children.erase(std::find(Children.begin(), Children.end(), Child));
}

void SimpleGUI::Base::RemoveChildren(Base * Child)
{
    auto iter = std::find_if(Children.begin(), Children.end(), [=](pBase child) { return child.get() == Child; });
    if (iter != Children.end())
        Children.erase(iter);
}

void SimpleGUI::Base::RemoveAllChildrens()
{
    for (auto &child : Children)
        if (HoveredControl == child.get())
            HoveredControl = nullptr;

    Children.clear();
}

SimpleGUI::Point SimpleGUI::Base::GetSize() const
{
    return Size;
}

void SimpleGUI::Base::SetSize(int x, int y)
{
    Size = { x, y };
}

void SimpleGUI::Base::SetSize(const Point & size)
{
    Size = size;
}

SimpleGUI::Base * SimpleGUI::Base::GetControlAt(int mouseX, int mouseY)
{
    for (auto &child : Children)
    {
        Base * elem = child.get();
        if (elem->IsHit({ mouseX, mouseY }))
        {
            return elem->GetControlAt(mouseX, mouseY);
        }
    }
    return this;
}

void SimpleGUI::Base::Draw(Skinner * skin)
{
    if (!skin)
        return;

    RecursiveDrawing(skin);
}

void SimpleGUI::Base::RecursiveDrawing(Skinner * skin)
{
    if (!skin)
        return;

    for (auto iter = Children.begin(); iter != Children.end(); iter++)
        (*iter)->Render(skin);
}

void SimpleGUI::Base::Render(Skinner * skin)
{
    if (this->IsVisible())
    {
        this->Draw(skin);
        RecursiveDrawing(skin);
    }
}

SimpleGUI::Base * SimpleGUI::Base::GetParent() const
{
    return Parent;
}

std::string SimpleGUI::Base::GetName() const
{
    return Name;
}

void SimpleGUI::Base::SetName(const std::string & val)
{
    Name = val;
}

const std::list<SimpleGUI::pBase> SimpleGUI::Base::GetChildrenList() const
{
    return Children;
}

bool SimpleGUI::Base::OnMouseMove(Point, Point)
{
    return false;
}

bool SimpleGUI::Base::OnMouseEnter()
{
    return false;
}

bool SimpleGUI::Base::OnMouseLeave()
{
    return false;
}

bool SimpleGUI::Base::OnMouseDown(MouseKeys, int, int)
{
    return false;
}

bool SimpleGUI::Base::OnMouseUp(MouseKeys, int, int)
{
    return false;
}

bool SimpleGUI::Base::OnKeyChar(wchar_t)
{
    return false;
}

bool SimpleGUI::Base::OnKeyUp(wchar_t)
{
    return false;
}

bool SimpleGUI::Base::OnKeyDown(wchar_t)
{
    return false;
}
