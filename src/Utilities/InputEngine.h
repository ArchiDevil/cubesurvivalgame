#pragma once

#include <Windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "ut.h"
#include "observer.h"
#include "singleton.h"
#include "System/System.h"

enum InputEventType
{
	IE_MouseUp,
	IE_MouseDown,
	IE_KeyDown,
	IE_KeyUp,
	IE_SystemKey,
	IE_SpecialKey,
};

struct InputEvent
{
	InputEvent(InputEventType type, long key) 
		: type(type), key(key) {}

	InputEventType type;
	long key;
};

class cInputEngine 
	: public singleton<cInputEngine>
	, public notifier<InputEvent>
	, public observer<SystemKeyMessage>
{
public:
	cInputEngine();
	~cInputEngine();

	bool Initialize(HWND hWnd, HINSTANCE hInstance);
	void GetKeys();

	bool IsMouseDown(MouseKeys key) const;
	bool IsMouseUp(MouseKeys key) const;
	bool IsMouseMoved() const;

	bool IsKeyDown(unsigned char Key) const;
	bool IsKeyUp(unsigned char Key) const;
	MouseInfo GetMouseInfo() const;

	bool handleEvent(const SystemKeyMessage & keyEvent) override;

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
