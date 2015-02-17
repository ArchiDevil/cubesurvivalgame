#pragma once

#include "Base.h"
#include "Table.h"

namespace SimpleGUI
{
    class List : public Base
    {
    public:
        List(Base * parent);

        virtual void Draw(Skinner * skin) override;
        Table * GetTable();

        virtual void SetSize(int x, int y) override;

    private:
        Table * pTable;

    };
}
