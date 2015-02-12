#pragma once

#include <GUI/GUI.h>

#include "Common.h"
#include "cAppStateBase.h"
#include "BlockWorkspace.h"

class WorkState : public appState
{
public:
    WorkState(int x_size, int y_size, int z_size);
    WorkState(const std::string & loadFile);
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

    void CreateGUI();
    bool ProcessInput(double dt);
    void ColorsCallBack(SimpleGUI::Text * t, SimpleGUI::ValueBox * val);

    std::unique_ptr<MeshEditor::BlockWorkspace> Workspace;
    SimpleGUI::Canvas * pCanvas;
    SimpleGUI::Skinner * pSkinner;

    MeshEditor::Brush curBrush;
    ShiftEngine::Sprite colorBox;

    Vector2I oldCoordinates;
    Vector2I newCoordinates;
    bool flag;
    bool geometryMode;

};
