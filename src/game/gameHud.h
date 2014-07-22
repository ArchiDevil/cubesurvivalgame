#pragma once

#include "../GraphicsEngine/Sprite.h"
#include "player/player.h"

class gameHUD
{
public:
	gameHUD();
	~gameHUD();

	void Initialize(cPlayer * pPlayer, int sw, int sh);
	void Draw();

private:
	ShiftEngine::Sprite * crosshair;
	ShiftEngine::Sprite * liHandItem;
	ShiftEngine::Sprite * liHandI; // обводка
	cPlayer * pPlayer;

};