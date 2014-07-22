#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "cSystem.h"

class cApplication
{
public:
	cApplication( HINSTANCE hInstance, int Width, int Height, LPCWSTR AppName );
	~cApplication();

	virtual bool Initialize()				= 0;
	virtual void Shutdown()					= 0;
	virtual int Run();
	virtual bool Frame()					= 0;

	virtual void ProcessMessage(MSG msg)	= 0;

	HWND GetHWND() const;
	HINSTANCE GetHINSTANCE() const;

protected:
	cSystem System;

private:
	bool CreateDefaultWindow(HINSTANCE hInstance, int Width, int Height, LPCWSTR AppName);

};