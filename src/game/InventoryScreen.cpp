#include "InventoryScreen.h"

InventoryScreen::InventoryScreen( int x_max, int y_max, Vector2D itemsStartPoint, int borderWidth )
{
	int screenWidth = ShiftEngine::GetContextManager()->GetParameters().screenWidth;
	int screenHeight = ShiftEngine::GetContextManager()->GetParameters().screenHeight;
	invSize = x_max * y_max;
	background = new ShiftEngine::Sprite(L"gui/inventory_back.png");
	int backgroundXStart = screenWidth / 2 - background->GetTextureDimensions().x / 2;
	int backgroundYStart = screenHeight / 2 - background->GetTextureDimensions().y / 2;
	background->SetPosition(Vector2F(backgroundXStart, backgroundYStart));
	items = new ShiftEngine::Sprite[invSize];
	for (int i = 0; i < invSize; i++)
	{
		int newXPos = itemsStartPoint.x + i % x_max + borderWidth * (i % x_max);
		int newYPos = itemsStartPoint.y + i / y_max + borderWidth * (i / y_max);
		items[i].SetPosition(Vector2F(newXPos, newYPos));
	}
}

InventoryScreen::~InventoryScreen()
{
	delete background;
	delete [] items;
}

void InventoryScreen::Draw()
{
	background->Draw();
	for (int i = 0; i < invSize; i++)
	{
		items[i].Draw();
	}
}