#include "List.h"

SimpleGUI::List::List(Base * parent)
    : Base(parent)
{
    SimpleGUI::Table * pt = new SimpleGUI::Table(this);
    pTable = pt;
    pt->SetSize(this->Size.x, this->Size.y);
}

void SimpleGUI::List::Draw(Skinner * skin)
{
    skin->DrawList(this);

    pTable->Draw(skin);
}

SimpleGUI::Table * SimpleGUI::List::GetTable()
{
    return pTable;
}

void SimpleGUI::List::SetSize(int x, int y)
{
    this->Size.x = x;
    this->Size.y = y;
    pTable->SetSize(x, y);
}
