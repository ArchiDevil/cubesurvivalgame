#pragma once

#include "Item.h"

#include "../game.h"
#include "../Entities/PlayerGameObject.h"

class Item;

class FoodItem : public Item
{
public:
    FoodItem(const std::string & name,
             const std::string & desc,
             ShiftEngine::IMeshDataPtr data,
             ShiftEngine::ITexturePtr ptr,
             int hunger,
             const std::string & imageName)
        : Item(ptr, data, name, desc, imageName)
        , hunger(hunger)
    {
    }

    bool UseOnPlayer() const override
    {
        auto * pPlayer = LostIsland::GetGamePtr()->Player;
        pPlayer->SetHunger(pPlayer->GetHunger() + GetHunger());
        return true;
    }

    ItemType GetType() const override
    {
        return ItemType::Food;
    }

    int GetHunger() const
    {
        return hunger;
    }

private:
    const int hunger = 0;

};
