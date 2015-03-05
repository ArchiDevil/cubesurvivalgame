#pragma once

#include "Base.h"

namespace SimpleGUI
{
    class ProgressBar : public Base
    {
    public:
        ProgressBar(Base * parent);
        virtual void Draw(Skinner * skin);

        void SetMinValue(int minVal);
        void SetMaxValue(int maxVal);

        int GetMinValue() const;
        int GetMaxValue() const;

        void SetValue(int val);
        int GetValue() const;

        void SetPercentValue(float percent);
        float GetPercentValue() const;

        void Step();
        int GetStepValue() const;
        void SetStepValue(int step);

        void SetColor(const MathLib::Vector3F & newColor);
        MathLib::Vector3F GetColor() const;

    private:
        int value;
        int stepValue;
        int minValue;
        int maxValue;
        MathLib::Vector3F color;
    };
}
