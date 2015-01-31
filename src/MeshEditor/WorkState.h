#pragma once

#include <GUI/GUI.h>

#include "cAppStateBase.h"
#include "BlockWorkspace.h"
#include "appclass.h"

class Application;

struct Brush
{
    Brush(Vector3F _Color = Vector3F(1.0f, 1.0f, 1.0f)) 
        : Color(_Color) 
    {}

    Vector3F Color;
};

class WorkState : public appState
{
public:
    WorkState(int x_size, int y_size, int z_size, SimpleGUI::Canvas * _pCanvas, SimpleGUI::Skinner * _pSkinner);
    WorkState(const std::string & loadFile, SimpleGUI::Canvas * _pCanvas, SimpleGUI::Skinner * _pSkinner);
    ~WorkState();

    virtual bool initState() override;
    virtual bool update(double dt) override;
    virtual bool render(double dt) override;
    virtual void onKill() override;
    virtual void onSuspend() override;
    virtual void onResume() override;

private:
    void MoveToGeometryMode();
    void MoveToColorMode();

    void FillByDefault();
    void CreateGUI();
    bool ProcessInput(double ElapsedTime);
    void ColorsCallBack(SimpleGUI::Text * t, SimpleGUI::ValueBox * val);

    MeshEditor::BlockWorkspace * Workspace;
    SimpleGUI::Canvas * pCanvas;
    SimpleGUI::Skinner * pSkinner;

    Brush curBrush;

    Vector2I oldCoordinates;
    Vector2I newCoordinates;
    bool flag;
    bool geometryMode;

};
