#include "ValueBox.h"

SimpleGUI::ValueBox::ValueBox(SimpleGUI::Base * parent)
    : Base(parent)
    , isMoving(false)
    , minValue(0)
    , maxValue(100)
    , value(0)
{
}

SimpleGUI::ValueBox::~ValueBox()
{
}

bool SimpleGUI::ValueBox::OnMouseDown(MouseKeys /*mb*/, int x, int /*y*/)
{
    isMoving = true;
    float percent = (float)x / this->Size.x;
    //and calculate new value
    value = (int)MathLib::LinearInterpolation((float)minValue, (float)maxValue, percent);
    value = MathLib::clamp(value, minValue, maxValue);
    OnValueChanged();
    return true;
}

bool SimpleGUI::ValueBox::OnMouseUp(MouseKeys /*mb*/, int /*x*/, int /*y*/)
{
    isMoving = false;
    OnValueChanged();
    return true;
}

bool SimpleGUI::ValueBox::OnMouseLeave()
{
    isMoving = false;
    OnValueChanged();
    return true;
}

void SimpleGUI::ValueBox::Draw(Skinner * skin)
{
    if (!skin)
        return;

    skin->DrawValueBox(this);
}

void SimpleGUI::ValueBox::OnValueChanged()
{
    if (onValueChangedHandler)
        onValueChangedHandler(this->value);
}

//////////////////////////////////////////////////////////////////////////
// Setters/Getters
//////////////////////////////////////////////////////////////////////////

int SimpleGUI::ValueBox::GetMinValue() const
{
    return minValue;
}

void SimpleGUI::ValueBox::SetMinValue(int val)
{
    minValue = val;
}

int SimpleGUI::ValueBox::GetValue() const
{
    return value;
}

void SimpleGUI::ValueBox::SetValue(int val)
{
    value = val;
}

int SimpleGUI::ValueBox::GetMaxValue() const
{
    return maxValue;
}

void SimpleGUI::ValueBox::SetMaxValue(int val)
{
    maxValue = val;
}

void SimpleGUI::ValueBox::SetOnValueChangedHandler(std::function<void(int)> handler)
{
    this->onValueChangedHandler = handler;
}
