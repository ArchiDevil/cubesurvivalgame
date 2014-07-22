#pragma once

#include "../GraphicsEngine/ShiftEngine.h"

using MathLib::Vector2D;

class InventoryScreen
{
public:
	InventoryScreen(int x_max, int y_max, Vector2D itemsStartPoint, int borderWidth);
	~InventoryScreen();

	void Draw();

private:
	ShiftEngine::Sprite * background;
	ShiftEngine::Sprite * items;

	int invSize;

};