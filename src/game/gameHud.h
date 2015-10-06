#pragma once

#include <MyGUI.h>

class GameHUD
{
public:
    GameHUD(MyGUI::Gui * guiModule);
    ~GameHUD();

    void Draw();
    void Update(double dt);
    void ReloadInventory();
    void OnCraftingProgress();

    void SwitchInventoryWindow();
    void SwitchCraftingWindow();

private:
    void OnCraftRequest(MyGUI::Widget * _sender);
    void OnSelectRecipe(MyGUI::Widget * _sender, size_t row);
    void OnCloseCraftingWindowClick(MyGUI::Widget * _sender, const std::string & eventName);
    void OnCloseInventoryWindowClick(MyGUI::Widget * _sender, const std::string & eventName);
    void OnInventoryImageClick(MyGUI::Widget * _sender);

    void CreateOtherElements();
    void CreateInventoryWindow();
    void CreateCraftingWindow();

    MyGUI::Gui *        guiModule = nullptr;
    MyGUI::Window *     craftingWindow = nullptr;
    MyGUI::Window *     inventoryWindow = nullptr;
    MyGUI::Widget *     rightHandPanel = nullptr;
    MyGUI::ImageBox *   rightHandImage = nullptr;
};
