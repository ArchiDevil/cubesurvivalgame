#pragma once

#include <GraphicsEngine/Sprite.h>
#include <MyGUI.h>

#include <memory>
#include <array>

class GameHUD
{
public:
    GameHUD(MyGUI::Gui * guiModule);
    ~GameHUD();

    void Draw();
    void Update(double dt);
    void ReloadInventory();
    void OnCraftingProgress();
    void SelectSlot(uint32_t slot);
    uint32_t GetSelectedSlot() const;

    void SwitchInventoryWindow();
    void SwitchCraftingWindow();

private:
    void OnCraftRequest(MyGUI::Widget * _sender);
    void OnSelectRecipe(MyGUI::Widget * _sender, size_t row);
    void OnCloseCraftingWindowClick(MyGUI::Widget * _sender, const std::string & eventName);
    void OnCloseInventoryWindowClick(MyGUI::Widget * _sender, const std::string & eventName);

    void CreateOtherElements();
    void CreateInventoryWindow();
    void CreateCraftingWindow();

    MyGUI::Gui *                                            guiModule = nullptr;
    MyGUI::Window *                                         craftingWindow = nullptr;
    MyGUI::Window *                                         inventoryWindow = nullptr;

    //TODO: remove all and replace on MyGUI widgets
    uint32_t                                                selectedSlot = 0;
    std::unique_ptr<ShiftEngine::Sprite>                    itemPanel = nullptr;
    std::unique_ptr<ShiftEngine::Sprite>                    selectedBorder = nullptr;
    std::array<std::unique_ptr<ShiftEngine::Sprite>, 10>    itemsSprites; // for now it's just only 10 items in inventory
};
