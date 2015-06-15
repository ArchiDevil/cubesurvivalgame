#pragma once

#include "System.h"

class Application
{
public:
    Application(int Width, int Height, LPCWSTR AppName);
    virtual ~Application();

    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual int Run();
    virtual bool Frame() = 0;

    virtual void ProcessMessage(MSG msg) = 0;

    HWND GetHWND() const;
    HINSTANCE GetHINSTANCE() const;

protected:
    cSystem System;

private:
    bool CreateDefaultWindow(int Width, int Height, LPCWSTR AppName);

};
