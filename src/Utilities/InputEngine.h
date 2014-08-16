#pragma once

#include <Windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "ut.h"
#include "singleton.h"
#include "notifier.h"

enum InputEvents
{
	IE_MouseUp,
	IE_MouseDown,
	IE_KeyDown,
	IE_KeyUp,
};

class cInputEngine : public singleton<cInputEngine>//, public notifier<InputEvents>
{
public:
	cInputEngine();
	~cInputEngine();

	bool Initialize(HWND hWnd, HINSTANCE hInstance);
	void GetKeys();

	bool IsMouseDown(MouseKeys key);
	bool IsMouseUp(MouseKeys key);
	bool IsMouseMoved();

	bool IsKeyDown(unsigned char Key);
	bool IsKeyUp(unsigned char Key);
	MouseInfo GetMouseInfo();

	void ProcEvent(MSG msg);

	//virtual void notifyAll();

private:
	IDirectInput8 * di; //DirectInput указатель
	IDirectInputDevice8 * keyboard;
	IDirectInputDevice8 * mouse;

	char curKeyBuffer[256]; //буфер для клавиатуры для DI
	char preKeyBuffer[256];

	MouseInfo curMouseBuffer1;
	MouseInfo preMouseBuffer1;

	DIMOUSESTATE curMouseBuffer;
	DIMOUSESTATE preMouseBuffer;

	HWND hWnd;

};
