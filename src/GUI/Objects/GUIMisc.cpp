#include "GUIMisc.h"

#include "Base.h"
#include "Canvas.h"
#include "../Skinner.h"
#include "../MainListener.h"

namespace SimpleGUI
{
    Base * HoveredControl;
    Base * FocusedControl;
    Canvas * ActiveCanvas;
    Skinner * ActiveSkinner;
    MainListener * ActiveListener;

    FloatSize CalculateScaling(int needX, int needY, int actualX, int actualY)
    {
        return FloatSize((float)needX / actualX, (float)needY / actualY);
    }

    void DrawUI()
    {
        if (!ActiveCanvas || !ActiveSkinner)
            return;

        ActiveCanvas->Render(ActiveSkinner);
    }

    void SetCanvas(Canvas * pCanvas)
    {
        ActiveCanvas = pCanvas;
        if (ActiveListener)
            ActiveListener->GetInputHandler()->SetCanvas(pCanvas);
    }

    void SetSkinner(Skinner * pSkinner)
    {
        ActiveSkinner = pSkinner;
    }

    void SetMainListener(MainListener * pListener)
    {
        ActiveListener = pListener;
        if (ActiveListener)
            ActiveListener->GetInputHandler()->SetCanvas(ActiveCanvas);
    }

}
