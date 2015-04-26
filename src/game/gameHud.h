#pragma once

#include <GraphicsEngine/Sprite.h>
#include <MyGUI.h>

#include <memory>
#include <array>

class GameHUD
{
public:
    GameHUD(MyGUI::Gui * guiModule);

    void Draw();
    void OnUserInventoryChange();
    void OnCraftingProgress();
    void SelectSlot(uint32_t slot);
    uint32_t GetSelectedSlot() const;

    void OpenInventoryWindow();
    void CloseInventoryWindow();

    void OpenCraftingWindow();
    void CloseCraftingWindow();

private:
    void OnCraftRequest(MyGUI::Widget * _sender);
    void OnSelectRecipe(MyGUI::Widget * _sender, size_t row);

    //TODO: remove all and replace on MyGUI widgets
    uint32_t                                                selectedSlot = 0;
    std::unique_ptr<ShiftEngine::Sprite>                    itemPanel = nullptr;
    std::unique_ptr<ShiftEngine::Sprite>                    selectedBorder = nullptr;
    std::array<std::unique_ptr<ShiftEngine::Sprite>, 10>    itemsSprites; // for now it's just only 10 items in inventory

    std::unique_ptr<ShiftEngine::Sprite>                    progressBarBack = nullptr;
    std::unique_ptr<ShiftEngine::Sprite>                    healthBar = nullptr;
    std::unique_ptr<ShiftEngine::Sprite>                    warmthBar = nullptr;
    std::unique_ptr<ShiftEngine::Sprite>                    hungerBar = nullptr;

    MyGUI::Gui *                                            guiModule = nullptr;

    MyGUI::Window *                                         craftingWindow = nullptr;
    MyGUI::Window *                                         inventoryWindow = nullptr;
};
