#pragma once

#include "Base.h"
#include "../Models/ListModel.h"

#include <functional>

namespace SimpleGUI
{
    class Table;

    class TableRow : public Base
    {
    public:
        TableRow(Table * parent, AnsiString _text);
        virtual bool OnMouseUp(MouseKeys mb, int innerX, int innerY) override;
        bool IsSelected() const;
        void Select();
        void Deselect();
        const AnsiString & GetString() const;

        virtual void Draw(Skinner * skin) override;

    private:
        AnsiString text = "";
        bool Selected = false;
        Table * typedParent = nullptr;
    };

    class Table : public Base, public IModelSubscriber
    {
        friend class TableRow;
    public:
        Table(Base * parent);

        virtual void Draw(Skinner * skin) override;
        ListModel * GetModel();
        const ListModel * GetModel() const;
        size_t GetSelectedRow() const;
        void UnselectAll();
        void SetOnRowSelectedHandler(std::function<void(size_t)> handlerFunc);

    private:
        virtual void OnModelChanged() override;

        void onRowSelected(TableRow * row);
        void Rebuild();

        std::unique_ptr<ListModel> model;
        std::vector<TableRow*> rows;

        std::function<void(size_t)> handler = nullptr;
        size_t selectedRow = (size_t)-1;
        bool changed = false;
    };
}
