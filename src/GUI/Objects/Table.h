#pragma once

#include "Base.h"

namespace SimpleGUI
{
    class TableRow : public Base
    {
    public:
        TableRow(Base * parent, AnsiString _text);
        virtual bool OnMouseUp(MouseKeys mb, int innerX, int innerY) override;
        bool IsSelected() const;
        void Select();
        void Deselect();
        AnsiString GetString() const;

        virtual void Draw(Skinner * skin) override;

    private:
        AnsiString text;
        bool Selected;
    };

    class Table : public Base
    {
    public:
        Table(Base * parent);
        ~Table();

        virtual void Draw(Skinner * skin) override;

        void AddRow(AnsiString str);
        void AddRow(AnsiString str, size_t pos);
        void RemoveRow(AnsiString str);
        void RemoveRow(size_t row);
        void Clear();
        TableRow * GetRow(size_t row);
        TableRow * GetSelectedRow();
        void UnselectAll();
        void onRowSelected(TableRow * row);

    private:
        TableRow * selectedRow;
        std::vector<TableRow*> rows;

    };
}
