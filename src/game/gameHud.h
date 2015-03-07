#pragma once

#include <GUI/GUI.h>

#include <memory>
#include <array>

class gameHUD
{
public:
	gameHUD();

	void Initialize(SimpleGUI::Canvas * pCanvas);
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
    void onCraftRequest();

	uint32_t selectedSlot = 0;
    std::unique_ptr<ShiftEngine::Sprite> itemPanel = nullptr;
    std::unique_ptr<ShiftEngine::Sprite> selectedBorder = nullptr;
	std::array<std::unique_ptr<ShiftEngine::Sprite>, 10> itemsSprites; // for now it's just only 10 items in inventory

	std::unique_ptr<ShiftEngine::Sprite> progressBarBack = nullptr;
    std::unique_ptr<ShiftEngine::Sprite> healthBar = nullptr;
    std::unique_ptr<ShiftEngine::Sprite> warmthBar = nullptr;
    std::unique_ptr<ShiftEngine::Sprite> hungerBar = nullptr;

    SimpleGUI::Window * pInventory = nullptr;
    SimpleGUI::Window * pCrafting = nullptr;
    SimpleGUI::List   * pRecipesList = nullptr;
    SimpleGUI::Canvas * pCanvas = nullptr;
};
