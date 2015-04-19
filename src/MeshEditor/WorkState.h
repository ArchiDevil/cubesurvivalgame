#pragma once

#include <Utilities/observer.h>
#include <Utilities/InputEngine.h>
#include <GraphicsEngine/ShiftEngine.h>

#include <MyGUI.h>
#include <MyGUI_DirectX11Platform.h>

#include "Common.h"
#include "cAppStateBase.h"
#include "BlockWorkspace.h"

class WorkState :
    public appState,
    observer < InputEvent >
{
public:
    WorkState(int x_size, int y_size, int z_size, MyGUI::Gui * pGui, MyGUI::DirectX11Platform * pPlatform);
    WorkState(const std::string & loadFile, MyGUI::Gui * pGui, MyGUI::DirectX11Platform * pPlatform);
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

    void SaveFile(MyGUI::Widget * _sender);
    void CancelSave(MyGUI::Widget * _sender);
    void ShowSaveDialog(MyGUI::Widget * _sender);

    void ShowColorDialog(MyGUI::Widget * _sender);
    void CloseColorDialog(MyGUI::Widget * _sender);
    void ColorChange(MyGUI::ScrollBar* _sender, size_t newVal);

    virtual bool handleEvent(const InputEvent & event);

    std::unique_ptr<MeshEditor::BlockWorkspace> Workspace;

    MyGUI::Gui *                pGui = nullptr;
    MyGUI::DirectX11Platform *  pPlatform = nullptr;

    MyGUI::Window *             pSaveWindow = nullptr;
    MyGUI::Window *             pColorWindow = nullptr;

    MyGUI::Button *             pSaveButton = nullptr;
    MyGUI::Button *             pColorButton = nullptr;

    MeshEditor::Brush           curBrush;
    ShiftEngine::Sprite         colorBox;

    Vector2I                    oldCoordinates;
    Vector2I                    newCoordinates;
    bool                        flag;
    bool                        geometryMode;

};
