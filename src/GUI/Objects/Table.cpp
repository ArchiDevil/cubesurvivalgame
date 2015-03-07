#include "Table.h"

SimpleGUI::TableRow::TableRow(Base * parent, AnsiString _text)
    : Base(parent)
    , text(_text)
    , Selected(false)
{
}

bool SimpleGUI::TableRow::OnMouseUp(MouseKeys mb, int /*innerX*/, int /*innerY*/)
{
    if (mb != 0) //left
        return false;

    Selected = true;
    Table * par = dynamic_cast<Table*>(Parent);
    if (!par) 
        throw std::exception();
    par->onRowSelected(this);
    return true;
}

bool SimpleGUI::TableRow::IsSelected() const
{
    return Selected;
}

void SimpleGUI::TableRow::Deselect()
{
    Selected = false;
}

void SimpleGUI::TableRow::Draw(Skinner * skin)
{
    skin->DrawListRow(this);
}

SimpleGUI::AnsiString SimpleGUI::TableRow::GetString() const
{
    return text;
}

void SimpleGUI::TableRow::Select()
{
    Selected = true;
}

//////////////////////////////////////////////////////////////////////////

SimpleGUI::Table::Table(Base * parent)
    : Base(parent)
    , selectedRow(nullptr)
{
}

SimpleGUI::Table::~Table()
{
}

void SimpleGUI::Table::Draw(Skinner * skin)
{
    Base::RecursiveDrawing(skin);
}

void SimpleGUI::Table::AddRow(AnsiString str)
{
    TableRow * pRow = new TableRow(this, str);

    pRow->SetSize(this->Size.x - 6, 16);
    pRow->SetPosition(3, rows.size() * 17 + 2);
    rows.push_back(pRow);
}

void SimpleGUI::Table::AddRow(AnsiString str, size_t pos)
{
    LOG_FATAL_ERROR("Fatal error");
}

void SimpleGUI::Table::RemoveRow(AnsiString str)
{
    auto iter = std::find_if(rows.begin(), rows.end(), [&](TableRow * row) 
    { 
        if (row->GetString() == str) 
            return true; 
        return false; 
    });

    RemoveChildren(*iter);

    if (iter != rows.end())
        rows.erase(iter);
}

void SimpleGUI::Table::RemoveRow(size_t row)
{
    RemoveChildren(rows[row]);
}

void SimpleGUI::Table::Clear()
{
    Base::RemoveAllChildrens();
    rows.clear();
}

SimpleGUI::TableRow * SimpleGUI::Table::GetRow(size_t row)
{
    if (row < rows.size())
        return rows[row];

    return nullptr;
}

SimpleGUI::TableRow * SimpleGUI::Table::GetSelectedRow()
{
    return selectedRow;
}

void SimpleGUI::Table::UnselectAll()
{
    for (TableRow * row : rows)
        if (row->IsSelected())
            row->Deselect();
}

void SimpleGUI::Table::onRowSelected(TableRow * row)
{
    selectedRow = row;
    UnselectAll();
    row->Select();
}
