#pragma once

#include <Windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <Utilities/ut.h>
#include <Utilities/observer.h>
#include <Utilities/singleton.h>

#include "Win32/System.h"

enum class InputEventType
{
    MouseUp,
    MouseDown,
    KeyDown,
    KeyUp,
    SystemKey,
};

struct InputEvent
{
    InputEvent(InputEventType type, long key)
        : type(type)
        , key(key)
    {
    }

    InputEventType type;
    long key;
};

class InputEngine
    : public singleton < InputEngine >
    , public notifier < InputEvent >
    , public observer < SystemKeyMessage >
{
public:
    InputEngine();
    ~InputEngine();

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
