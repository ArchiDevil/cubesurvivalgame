#pragma once

#include "../GUI/GUI.h"

#include "cAppStateBase.h"
#include "cWorkspace.h"
#include "appclass.h"

class Application;

struct Brush
{
	Brush(Vector3F _Color = Vector3F(1.0f, 1.0f, 1.0f)) : Color(_Color) {};
	Vector3F Color;
};

class cWorkState : public appState
{
public:
	cWorkState(int x_size, int y_size, int z_size, SimpleGUI::Canvas * _pCanvas, SimpleGUI::Skinner * _pSkinner);
	cWorkState(const std::wstring & loadFile, SimpleGUI::Canvas * _pCanvas, SimpleGUI::Skinner * _pSkinner);
	~cWorkState();

	virtual bool initState();
	virtual bool update( double dt );
	virtual bool render( double dt );
	virtual void onKill();
	virtual void onSuspend();
	virtual void onResume();
	
private:
	void MoveToGeometryMode();
	void MoveToColorMode();

	void FillByDefault();
	void CreateGUI();
	bool ProcessInput(double ElapsedTime);
	void ColorsCallBack(SimpleGUI::Text * t, SimpleGUI::ValueBox * val);

	cWorkspace * Workspace;
	SimpleGUI::Canvas * pCanvas;
	SimpleGUI::Skinner * pSkinner;

	Brush curBrush;

	Vector2I oldCoordinates;
	Vector2I newCoordinates;
	bool flag;
	bool geometryMode;
	
};
