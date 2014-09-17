#pragma once

#include <GraphicsEngine/Sprite.h>

class gameHUD
{
public:
	gameHUD();

	void Initialize(int sw, int sh);
	void Draw();

private:
	std::unique_ptr<ShiftEngine::Sprite> crosshair;
	std::unique_ptr<ShiftEngine::Sprite> liHandItem;
	std::unique_ptr<ShiftEngine::Sprite> liHandI;

	size_t width, height;

};
