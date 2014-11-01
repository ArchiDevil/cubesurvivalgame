#include "InputEngine.h"

cInputEngine::cInputEngine()
	: mouse(nullptr)
	, keyboard(nullptr)
	, di(nullptr)
	, hWnd(0)
{
}

cInputEngine::~cInputEngine()
{
	if (keyboard)
		keyboard->Release();
	if (mouse)
		mouse->Release();
	if (di)
		di->Release();
}

bool cInputEngine::Initialize(HWND hWnd, HINSTANCE hInstance)
{
	this->hWnd = hWnd;

	if (FAILED(DirectInput8Create(hInstance,	//инстанция окна, откуда забираем кнопки
		DIRECTINPUT_VERSION,				//всегда передаем такой параметр
		IID_IDirectInput8,					//аналогично - всегда такой параметр
		(void**)&di,						//указатель на устройство
		NULL)))								//всегда NULL
	{
		return false;
	}

	di->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	di->CreateDevice(GUID_SysMouse, &mouse, NULL);

	if (!keyboard || !mouse)
		return false;

	keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	keyboard->SetDataFormat(&c_dfDIKeyboard);
	mouse->SetDataFormat(&c_dfDIMouse);

	ZeroMemory(&curKeyBuffer, sizeof(char) * 256);
	ZeroMemory(&curMouseBuffer, sizeof(curMouseBuffer));

	return true;
}

void cInputEngine::GetKeys()
{
	preMouseBuffer = curMouseBuffer;
	memcpy(preKeyBuffer, curKeyBuffer, 256);

	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(curKeyBuffer), &curKeyBuffer);

	preMouseBuffer1 = curMouseBuffer1;
	curMouseBuffer1 = GetMouseInfo();

	mouse->Acquire();
	mouse->GetDeviceState(sizeof(curMouseBuffer), &curMouseBuffer);
}

MouseInfo cInputEngine::GetMouseInfo() const
{
	POINT pt;
	POINT pt2;
	GetCursorPos(&pt);
	pt2 = pt;
	ScreenToClient(hWnd, &pt2);
	MouseInfo out(curMouseBuffer.lX, curMouseBuffer.lY, curMouseBuffer.lZ, pt.x, pt.y, pt2.x, pt2.y);
	return out;
}

bool cInputEngine::IsMouseUp(MouseKeys key) const
{
	if (preMouseBuffer.rgbButtons[(int)key] && !curMouseBuffer.rgbButtons[(int)key])
		return true;
	return false;
}

bool cInputEngine::IsMouseDown(MouseKeys key) const
{
	if (preMouseBuffer.rgbButtons[(int)key] && curMouseBuffer.rgbButtons[(int)key])
		return true;
	return false;
}

bool cInputEngine::IsKeyDown(unsigned char Key) const
{
	if (curKeyBuffer[Key] && preKeyBuffer[Key])
		return true;
	return false;
}

bool cInputEngine::IsKeyUp(unsigned char Key) const
{
	if (!curKeyBuffer[Key] && preKeyBuffer[Key])
		return true;
	return false;
}

bool cInputEngine::IsMouseMoved() const
{
	if (curMouseBuffer.lX != 0 || curMouseBuffer.lY != 0 || curMouseBuffer.lZ != 0)
		return true;
	return false;
}

bool cInputEngine::handleEvent(const SystemKeyMessage & keyEvent)
{
	switch (keyEvent.system_key)
	{
	case SK_BACKSPACE:
	case SK_ENTER:
	case SK_ESC:
	case SK_TAB:
		notifyAll(InputEvent(IE_SpecialKey, keyEvent.system_key));
		break;
	case SK_CHAR:
		notifyAll(InputEvent(IE_SystemKey, keyEvent.character));
		return true;
	default:
		return false;
	}
	return false;
}
