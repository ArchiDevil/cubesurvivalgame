#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "../observer.h"

enum AppState
{
	AS_Running,
	AS_Inactive
};

enum SystemKey
{
	SK_BACKSPACE,
	SK_ENTER,
	SK_ESC,
	SK_TAB,
	SK_CHAR
};

struct SystemKeyMessage
{
	SystemKeyMessage(SystemKey system_key = SK_CHAR, unsigned long character = 0) 
		: system_key(system_key)
		, character(character)
	{
	}

	SystemKey system_key;
	unsigned long character;
};

class cSystem : public notifier<SystemKeyMessage>
{
public:
	cSystem();
	~cSystem();

	bool InitializeWindow(int Width, int Height, LPCWSTR AppName);
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

LRESULT CALLBACK WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
