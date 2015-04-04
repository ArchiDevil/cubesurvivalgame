#include "Table.h"

SimpleGUI::TableRow::TableRow(Table * parent, AnsiString text)
    : Base(parent)
    , text(text)
    , typedParent(parent)
{
}

bool SimpleGUI::TableRow::OnMouseUp(MouseKeys mb, int /*innerX*/, int /*innerY*/)
{
    if (mb != 0) //left
        return false;

    Selected = true;
    if (!typedParent)
    {
        return false;
    }
    typedParent->onRowSelected(this);
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

void SimpleGUI::TableRow::Select()
{
    Selected = true;
}

void SimpleGUI::TableRow::Draw(Skinner * skin)
{
    skin->DrawListRow(this);
}

const SimpleGUI::AnsiString & SimpleGUI::TableRow::GetString() const
{
    return text;
}

//////////////////////////////////////////////////////////////////////////

SimpleGUI::Table::Table(Base * parent)
    : Base(parent)
    , model(new ListModel(this))
    , IModelSubscriber(model.get())
{
}

void SimpleGUI::Table::Draw(Skinner * skin)
{
    if (changed)
        Rebuild();

    Base::RecursiveDrawing(skin);
}

void SimpleGUI::Table::UnselectAll()
{
    for (TableRow * row : rows)
        if (row->IsSelected())
            row->Deselect();

    selectedRow = (size_t)-1;
}

void SimpleGUI::Table::onRowSelected(TableRow * row)
{
    UnselectAll();

    auto iter = std::find(rows.begin(), rows.end(), row);
    if (iter != rows.end())
        selectedRow = std::abs(std::distance(iter, rows.begin()));

    (*iter)->Select();

    if (handler)
        handler(selectedRow);
}

void SimpleGUI::Table::SetOnRowSelectedHandler(std::function<void(size_t)> handlerFunc)
{
    handler = handlerFunc;
}

void SimpleGUI::Table::OnModelChanged()
{
    changed = true;
}

SimpleGUI::ListModel * SimpleGUI::Table::GetModel()
{
    return model.get();
}

const SimpleGUI::ListModel * SimpleGUI::Table::GetModel() const
{
    return model.get();
}

void SimpleGUI::Table::Rebuild()
{
    for (TableRow * row : rows)
        RemoveChildren(row);

    rows.clear();

    for (size_t i = 0; i < model->Count(); ++i)
    {
        //TODO: not so good, but may be used now
        rows.push_back(new TableRow(this, model->Get(i).GetName()));
        TableRow * tr = rows.back();
        int rowMargin = 2;
        tr->SetPosition({ rowMargin, (int)i * 17 + 2 }); // margin of 2 and 17 pixels of height
        tr->SetSize({ GetSize().x - rowMargin * 2, 17 }); // 17?
    }

    changed = false;
}

size_t SimpleGUI::Table::GetSelectedRow() const
{
    return selectedRow;
}
