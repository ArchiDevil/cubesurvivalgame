#include "Table.h"

SimpleGUI::TableRow::TableRow( Base * parent, AnsiString _text )
	: Base(parent), text(_text), Selected(false)
{}

bool SimpleGUI::TableRow::OnMouseUp( int mb, int /*innerX*/, int /*innerY*/ )
{
	if(mb != 0) //left
		return false;

	Selected = true;
	Table * par = dynamic_cast<Table*>(Parent);
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

void SimpleGUI::TableRow::Draw( Skinner * skin )
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

SimpleGUI::Table::Table( Base * parent )
	: Base(parent), rows(0) {}

SimpleGUI::Table::~Table() {}

void SimpleGUI::Table::Draw( Skinner * skin )
{
	Base::RecursiveDrawing(skin);
}

void SimpleGUI::Table::AddRow( AnsiString str )
{
	TableRow * pRow = new TableRow(this, str);

	pRow->SetSize(this->Size.x - 6, 16);
	pRow->SetPosition(3, rows * 17 + 2);

	rows++;
}

void SimpleGUI::Table::RemoveRow( AnsiString str )
{
	LOG_FATAL_ERROR("Fatal error");
}

void SimpleGUI::Table::Clear()
{
	Base::RemoveAllChildrens();
}

SimpleGUI::TableRow * SimpleGUI::Table::GetRow( int /*num*/ )
{
	LOG_FATAL_ERROR("Fatal error");
}

SimpleGUI::TableRow * SimpleGUI::Table::GetSelectedRow()
{
	for (auto iter = Children.begin(); iter != Children.end(); iter++)
	{
		Base * elem = (*iter).get();
		TableRow * ptr = dynamic_cast<TableRow*>(elem);

		if(!ptr)
			continue;

		if(ptr->IsSelected())
			return ptr;
	}

	return nullptr;
}

void SimpleGUI::Table::UnselectAll()
{
	for (auto iter = Children.begin(); iter != Children.end(); iter++)
	{
		Base * elem = (*iter).get();
		TableRow * ptr = dynamic_cast<TableRow*>(elem);

		if(!ptr)
			continue;

		if(ptr->IsSelected())
			ptr->Deselect();
	}
}

void SimpleGUI::Table::onRowSelected( TableRow * row )
{
	UnselectAll();
	row->Select();
}
