#pragma once

#include "Listener.h"
#include "../Utilities/InputEngine.h"

namespace SimpleGUI
{
	class MainListener : public Listener
	{
	public:
		MainListener(Canvas * _pCanvas, cInputEngine * _eng, std::vector<wchar_t> & _charQueue);
		virtual void Update();

	private:
		cInputEngine * Engine;
		std::vector<wchar_t> & charQueue;

	};
}