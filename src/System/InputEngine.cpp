#include "InputEngine.h"

InputEngine::InputEngine()
    : mouse(nullptr)
    , keyboard(nullptr)
    , di(nullptr)
    , hWnd(0)
{
}

InputEngine::~InputEngine()
{
    if (keyboard)
        keyboard->Release();
    if (mouse)
        mouse->Release();
    if (di)
        di->Release();
}

bool InputEngine::Initialize(HWND hWnd, HINSTANCE hInstance)
{
    this->hWnd = hWnd;

    if (FAILED(DirectInput8Create(hInstance,    //инстанция окна, откуда забираем кнопки
        DIRECTINPUT_VERSION,                    //всегда передаем такой параметр
        IID_IDirectInput8,                      //аналогично - всегда такой параметр
        (void**)&di,                            //указатель на устройство
        NULL)))                                 //всегда NULL
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

void InputEngine::GetKeys()
{
    preMouseBuffer = curMouseBuffer;
    memcpy(preKeyBuffer, curKeyBuffer, 256);

    keyboard->Acquire();
    keyboard->GetDeviceState(sizeof(curKeyBuffer), &curKeyBuffer);

    preMouseBuffer1 = curMouseBuffer1;
    curMouseBuffer1 = GetMouseInfo();

    mouse->Acquire();
    mouse->GetDeviceState(sizeof(curMouseBuffer), &curMouseBuffer);

    for (int i = 0; i < 256; ++i)
    {
        if (curKeyBuffer[i] && !preKeyBuffer[i])
            notifyAll(InputEvent(InputEventType::KeyDown, i));
        if (!curKeyBuffer[i] && preKeyBuffer[i])
            notifyAll(InputEvent(InputEventType::KeyUp, i));
    }
}

MouseInfo InputEngine::GetMouseInfo() const
{
    POINT pt;
    POINT pt2;
    GetCursorPos(&pt);
    pt2 = pt;
    ScreenToClient(hWnd, &pt2);
    MouseInfo out(curMouseBuffer.lX, curMouseBuffer.lY, curMouseBuffer.lZ, pt.x, pt.y, pt2.x, pt2.y);
    return out;
}

bool InputEngine::IsMouseUp(MouseKeys key) const
{
    if (preMouseBuffer.rgbButtons[(int)key] && !curMouseBuffer.rgbButtons[(int)key])
        return true;
    return false;
}

bool InputEngine::IsMouseDown(MouseKeys key) const
{
    if (preMouseBuffer.rgbButtons[(int)key] && curMouseBuffer.rgbButtons[(int)key])
        return true;
    return false;
}

bool InputEngine::IsKeyDown(unsigned char Key) const
{
    if (curKeyBuffer[Key] && preKeyBuffer[Key])
        return true;
    return false;
}

bool InputEngine::IsKeyUp(unsigned char Key) const
{
    if (!curKeyBuffer[Key] && preKeyBuffer[Key])
        return true;
    return false;
}

bool InputEngine::IsMouseMoved() const
{
    if (curMouseBuffer.lX != 0 || curMouseBuffer.lY != 0 || curMouseBuffer.lZ != 0)
        return true;
    return false;
}

bool InputEngine::handleEvent(const SystemKeyMessage & keyEvent)
{
    notifyAll(InputEvent(InputEventType::SystemKey, keyEvent.character));
    return true;

}
