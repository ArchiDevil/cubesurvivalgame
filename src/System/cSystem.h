#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

enum AppState
{
	AS_Running,
	AS_Inactive
};

class cSystem
{
public:
	cSystem();
	~cSystem();

	bool InitializeWindow(HINSTANCE hInstance, int Width, int Height, LPCWSTR AppName);
	HWND GetHWND() const;
	HINSTANCE GetHINSTANCE() const;
	AppState GetState() const;

	LRESULT CALLBACK MessageHandler(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	AppState State;

	HWND WindowHandler;
	HINSTANCE WindowInstance;
};

static cSystem * System;

LRESULT CALLBACK WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam); //MUST BE OUTSIDE!!!