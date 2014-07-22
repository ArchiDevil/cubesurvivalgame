#include "GUIMisc.h"

namespace SimpleGUI
{
	Base * HoveredControl;
	Base * FocusedControl;

	FloatSize CalculateScaling( int needX, int needY, int actualX, int actualY )
	{
		return FloatSize((float)needX / actualX, (float)needY / actualY);
	}

}