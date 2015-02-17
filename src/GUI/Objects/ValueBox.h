#pragma once

#include <functional>

#include "Base.h"

namespace SimpleGUI
{
    class ValueBox : public Base
    {
    public:
        ValueBox(SimpleGUI::Base * parent);
        ~ValueBox();

        virtual bool OnMouseDown(MouseKeys mb, int x, int y) override;
        virtual bool OnMouseUp(MouseKeys mb, int x, int y) override;
        virtual bool OnMouseLeave() override;
        virtual void Draw(Skinner * skin) override;

        int GetMinValue() const;
        void SetMinValue(int val);

        int GetValue() const;
        void SetValue(int val);

        int GetMaxValue() const;
        void SetMaxValue(int val);

        void SetOnValueChangedHandler(std::function<void(int)> handler);

    protected:
        void OnValueChanged();

    private:
        bool isMoving;
        int value;
        int maxValue;
        int minValue;

        std::function<void(int)> onValueChangedHandler;
    };
}
