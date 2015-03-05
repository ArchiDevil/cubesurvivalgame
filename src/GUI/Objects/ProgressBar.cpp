#include "ProgressBar.h"

SimpleGUI::ProgressBar::ProgressBar(Base * parent)
    : Base(parent)
    , value(0)
    , minValue(0)
    , maxValue(0)
    , stepValue(1)
    , color(1.0f, 1.0f, 1.0f)
{
}

void SimpleGUI::ProgressBar::Draw(Skinner * skin)
{
    if (!skin)
        return;

    skin->DrawProgressBar(this);
}

void SimpleGUI::ProgressBar::SetMinValue(int minVal)
{
    if (minVal > maxValue)
        return;

    minValue = minVal;
    value = MathLib::clamp(value, minValue, maxValue);
}

void SimpleGUI::ProgressBar::SetMaxValue(int maxVal)
{
    if (maxVal < minValue)
        return;

    maxValue = maxVal;
    value = MathLib::clamp(value, minValue, maxValue);
}

int SimpleGUI::ProgressBar::GetMinValue() const
{
    return minValue;
}

int SimpleGUI::ProgressBar::GetMaxValue() const
{
    return maxValue;
}

void SimpleGUI::ProgressBar::SetPercentValue(float percent)
{
    percent = MathLib::clamp(percent, 0.0f, 100.0f);
    value = (int)((float)(maxValue - minValue) * percent / 100.0f + (float)minValue);
}

float SimpleGUI::ProgressBar::GetPercentValue() const
{
    return (float)(value - minValue) / (float)(maxValue - minValue) * 100.0f;
}

void SimpleGUI::ProgressBar::Step()
{
    value += stepValue;
    value = MathLib::clamp(value, minValue, maxValue);
}

int SimpleGUI::ProgressBar::GetStepValue() const
{
    return stepValue;
}

void SimpleGUI::ProgressBar::SetStepValue(int step)
{
    stepValue = step;
}

void SimpleGUI::ProgressBar::SetValue(int val)
{
    value = MathLib::clamp(val, minValue, maxValue);
}

int SimpleGUI::ProgressBar::GetValue() const
{
    return value;
}

void SimpleGUI::ProgressBar::SetColor(const MathLib::Vector3F & newColor)
{
    color = newColor;
}

MathLib::Vector3F SimpleGUI::ProgressBar::GetColor() const
{
    return color;
}
