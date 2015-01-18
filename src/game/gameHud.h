#pragma once

#include <GraphicsEngine/Sprite.h>

#include <memory>
#include <array>

class gameHUD
{
public:
	gameHUD();

	void Initialize();
	void Draw();
	void OnUserInventoryChange();
	void SelectSlot(uint32_t slot);
	uint32_t GetSelectedSlot() const;

private:
	//std::unique_ptr<ShiftEngine::Sprite> liHandItem;
	//std::unique_ptr<ShiftEngine::Sprite> liHandI;

	// new GUI
	uint32_t selectedSlot;
	std::unique_ptr<ShiftEngine::Sprite> itemPanel;
	std::unique_ptr<ShiftEngine::Sprite> selectedBorder;
	std::array<std::unique_ptr<ShiftEngine::Sprite>, 10> itemsSprites; // for now it's just only 10 items in inventory

	std::unique_ptr<ShiftEngine::Sprite> progressBarBack;
	std::unique_ptr<ShiftEngine::Sprite> healthBar;
	std::unique_ptr<ShiftEngine::Sprite> warmthBar;
	std::unique_ptr<ShiftEngine::Sprite> hungerBar;

};
