#pragma once

#include "../../Utilities/ut.h"
#include "../../MathLib/math.h"

#include <memory>

using MathLib::Vector2I;
using MathLib::Vector2F;

namespace SimpleGUI
{
	class Base;

	typedef std::shared_ptr<Base> pBase;

	extern Base *			HoveredControl;
	extern Base *			FocusedControl;

	typedef Vector2I		Point;
	typedef Vector2F		FloatSize;
	typedef std::string		AnsiString;
	typedef std::wstring	UnicodeString;	//cannot have 32-bit on Windows system

	FloatSize CalculateScaling(int needX, int needY, int actualX, int actualY);

	struct Area
	{
		Area(int _top, int _bottom, int _left, int _right) 
			: Top(_top), Bottom(_bottom), Left(_left), Right(_right) 
		{};

		int Top, Bottom, Left, Right;
	};

	enum EventType
	{
		m_MOUSEBEGIN,
		EventClick,
		EventMove,
		m_MOUSEEND,	//пока хватит
		EventChar,
	};

	struct Event
	{
		Event(EventType _Type, MouseInfo _MI, unsigned char _Key) 
			: Type(_Type), MI(_MI), Key(_Key) 
		{};

		EventType Type;
		MouseInfo MI;
		unsigned char Key;
	};
}