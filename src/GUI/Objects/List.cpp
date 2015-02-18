#include "List.h"

SimpleGUI::List::List(Base * parent)
    : Base(parent)
{
    SimpleGUI::Table * pt = new SimpleGUI::Table(this);
    pTable = pt;
    pt->SetSize(GetSize());
}

void SimpleGUI::List::Draw(Skinner * skin)
{
    skin->DrawList(this);
}

SimpleGUI::Table * SimpleGUI::List::GetTable()
{
    return pTable;
}

void SimpleGUI::List::SetSize(int x, int y)
{
    Base::SetSize(x, y);
    pTable->SetSize(x, y);
}
