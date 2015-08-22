#include "gameHud.h"

#include "game.h"
#include "Entities/GameObjectInventory.h"
#include "Entities/PlayerGameObject.h"
#include "Items/Item.h"
#include "CraftingManager.h"

#include <GraphicsEngine/ShiftEngine.h>

using ShiftEngine::Sprite;
using namespace MyGUI;

GameHUD::GameHUD(MyGUI::Gui * guiModule)
    : guiModule(guiModule)
{
    CreateInventoryWindow();
    CreateCraftingWindow();
    CreateOtherElements();
}

void GameHUD::Draw()
{
    ShiftEngine::IContextManager * pCtxMgr = ShiftEngine::GetContextManager();
    auto settings = pCtxMgr->GetEngineSettings();
    int screenWidth = settings.screenWidth;
    int screenHeight = settings.screenHeight;

    itemPanel->Draw();
    selectedBorder->SetPosition({ screenWidth / 2 - itemPanel->GetTextureDimensions().x / 2 + 24.0f + selectedSlot*48.0f, screenHeight - 30.0f });
    selectedBorder->Draw();

    auto pPlayerInventory = LostIsland::GetGamePtr()->player->GetInventoryPtr();
    const std::vector<SlotUnit> &items = pPlayerInventory->GetItems();
    for (size_t i = 0; i < itemsSprites.size(); ++i)
    {
        itemsSprites[i]->Draw();
        if (items[i].count)
        {
            int xpos = screenWidth / 2 - (int)itemPanel->GetTextureDimensions().x / 2 + 25 + i * 48;
            int ypos = screenHeight - 40;
            pCtxMgr->GetFontManager()->DrawTextTL(std::to_string(items[i].count), xpos, ypos);
        }
    }
}

void GameHUD::OnUserInventoryChange()
{
    auto pGame = LostIsland::GetGamePtr();
    auto & pItemMgr = pGame->itemMgr;

    for (int i = 0; i < 10; ++i)
    {
        itemsSprites[i]->SetTexture(ShiftEngine::ITexturePtr(nullptr));
    }

    const std::vector<SlotUnit> &items = pGame->player->GetInventoryPtr()->GetItems();
    size_t maxi = items.size();
    if (maxi > 10)
        maxi = 10;

    for (size_t i = 0; i < maxi; ++i)
    {
        auto item = pItemMgr->GetItemById(items[i].itemId);
        if (!item)
            continue;

        itemsSprites[i]->SetTexture(item->GetTexturePtr());
        itemsSprites[i]->SetSizeInPixels(44, 44);
    }
}

void GameHUD::SelectSlot(uint32_t slot)
{
    selectedSlot = slot;
}

uint32_t GameHUD::GetSelectedSlot() const
{
    return selectedSlot;
}

void GameHUD::SwitchInventoryWindow()
{
    if (!inventoryWindow)
        return;

    inventoryWindow->setVisible(!inventoryWindow->getVisible());
}

void GameHUD::SwitchCraftingWindow()
{
    if (!craftingWindow)
        return;

    craftingWindow->setVisible(!craftingWindow->getVisible());
}

void GameHUD::OnCraftingProgress()
{
    CraftingManager * pCraftingMgr = LostIsland::GetGamePtr()->craftingMgr.get();
    auto recipesVec = pCraftingMgr->GetAllRecipes();

    ListBox * list = craftingWindow->findWidget("recipes_list")->castType<ListBox>();
    if (!list)
        return;

    auto & pItemMgr = LostIsland::GetGamePtr()->itemMgr;

    list->removeAllItems();
    for (auto& recipe : recipesVec)
    {
        Item * pItem = pItemMgr->GetItemById(recipe.second.producedItem);
        if (!pItem)
            continue;

        list->addItem(pItem->GetName(), recipe.first);
    }
}

void GameHUD::OnCraftRequest(Widget * _sender)
{
    auto * pGame = LostIsland::GetGamePtr();
    ListBox * list = craftingWindow->findWidget("recipes_list")->castType<ListBox>();
    if (!list)
        return;

    size_t selectedRow = list->getIndexSelected();
    if (selectedRow == ITEM_NONE)
        return;

    const std::string * recipeName = list->getItemDataAt<std::string>(selectedRow);
    const Recipe & recipe = pGame->craftingMgr->GetRecipeByName(*recipeName);

    // TODO: notify user that he canot craft this item if he cannot craft :3
    if (!pGame->craftingMgr->Craft(recipe))
    {
        // do something
    }
    OnUserInventoryChange();
}

void GameHUD::OnSelectRecipe(Widget * _sender, size_t row)
{
    auto &pCraftingMgr = LostIsland::GetGamePtr()->craftingMgr;
    auto &pItemMgr = LostIsland::GetGamePtr()->itemMgr;
    auto pPlayer = LostIsland::GetGamePtr()->player;

    if (row == ITEM_NONE)
        return;

    // update icon, name and description
    ListBox * list = craftingWindow->findWidget("recipes_list")->castType<ListBox>();
    if (!list)
        return;

    const std::string * recipeName = list->getItemDataAt<std::string>(row);
    const Recipe &recipe = pCraftingMgr->GetRecipeByName(*recipeName);

    Item * pItem = pItemMgr->GetItemById(recipe.producedItem);
    if (!pItem)
        LOG_ERROR("Unable to find item: ", recipe.producedItem);

    ImageBox * itemImage = craftingWindow->findWidget("item_image")->castType<ImageBox>();
    TextBox * itemName = craftingWindow->findWidget("item_name")->castType<TextBox>();
    TextBox * itemDescription = craftingWindow->findWidget("item_description")->castType<TextBox>();

    itemImage->setImageTexture(pItem->GetImageFile());
    itemName->setCaption(pItem->GetName());
    itemDescription->setCaption(pItem->GetDescription());

    ImageBox * ingredientImage = nullptr;
    TextBox * ingredientName = nullptr;
    TextBox * ingredientCount = nullptr;

    for (size_t i = 0; i < recipe.itemsToCraft.size(); ++i)
    {
        ingredientImage = craftingWindow->findWidget("ingredient_icon_" + std::to_string(i))->castType<ImageBox>();
        ingredientName = craftingWindow->findWidget("ingredient_name_" + std::to_string(i))->castType<TextBox>();
        ingredientCount = craftingWindow->findWidget("ingredient_count_" + std::to_string(i))->castType<TextBox>();

        auto &pair = recipe.itemsToCraft[i];
        if (!pair.first || !pair.second)
        {
            ingredientImage->setImageTexture("");
            ingredientName->setCaption("");
            ingredientCount->setCaption("");
            continue;
        }

        pItem = pItemMgr->GetItemById(pair.first);
        if (!pItem)
        {
            ingredientImage->setImageTexture("");
            ingredientName->setCaption("");
            ingredientCount->setCaption("");
            continue;
        }

        const SlotUnit & slot = pPlayer->GetInventoryPtr()->FindSlotWithItem(pair.first);
        if (!slot.itemId || slot.count < pair.second)
        {
            ingredientImage->setAlpha(0.5f);
            ingredientName->setTextColour(Colour(1.0f, 0.0f, 0.0f));
            ingredientCount->setTextColour(Colour(1.0f, 0.0f, 0.0f));
        }
        else
        {
            ingredientImage->setAlpha(1.0f);
            ingredientName->setTextColour(Colour(0.0f, 0.0f, 0.0f));
            ingredientCount->setTextColour(Colour(0.0f, 0.0f, 0.0f));
        }

        ingredientImage->setImageTexture(pItem->GetImageFile());
        ingredientName->setCaption(pItem->GetName());
        ingredientCount->setCaption(std::to_string(pair.second));
    }
}

void GameHUD::OnCloseCraftingWindowClick(MyGUI::Widget * _sender, const std::string & eventName)
{
    craftingWindow->setVisible(false);
}

void GameHUD::CreateOtherElements()
{
    auto settings = ShiftEngine::GetContextManager()->GetEngineSettings();
    int screenWidth = settings.screenWidth;
    int screenHeight = settings.screenHeight;

    itemPanel.reset(new Sprite(L"gui/itemPanel.png"));
    float xPos = (float)screenWidth / 2.0f;
    itemPanel->SetPosition({ xPos, screenHeight - 30.0f });

    selectedBorder.reset(new Sprite(L"gui/selecteditem.png"));
    selectedBorder->SetPosition({ screenWidth / 2 - itemPanel->GetTextureDimensions().x / 2 + 24.0f + selectedSlot*48.0f, screenHeight - 30.0f });
    selectedBorder->SetSizeInPixels(48, 48);

    for (size_t i = 0; i < itemsSprites.size(); ++i)
    {
        auto & sprite = itemsSprites[i];
        sprite.reset(new Sprite());
        sprite->SetPosition({ screenWidth / 2 - itemPanel->GetTextureDimensions().x / 2 + 24.0f + i*48.0f, screenHeight - 30.0f });
    }

    const char * names[] = { "health_bar", "warmth_bar", "hunger_bar" };
    const Colour colors[] = { { 1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f, 0.8f }, { 0.8f, 0.8f, 0.0f } };

    for (int i = 0; i < 3; ++i)
    {
        ProgressBar * bar = guiModule->createWidget<ProgressBar>("ProgressBar",
                                                                 IntCoord(8, screenHeight - 90 + i * 30, 200, 24),
                                                                 Align::Default,
                                                                 "Overlapped",
                                                                 names[i]);

        bar->setProgressRange(100);
        bar->setProgressPosition(50);
        bar->setColour(colors[i]);
    }
}

void GameHUD::CreateInventoryWindow()
{
    inventoryWindow = guiModule->createWidget<Window>("WindowCX", IntCoord(300, 300, 600, 330), Align::Default, "Overlapped");
    inventoryWindow->setMovable(false);
    inventoryWindow->setVisible(false);
}

void GameHUD::CreateCraftingWindow()
{
    craftingWindow = guiModule->createWidget<Window>("WindowCX", IntCoord(300, 300, 600, 330), Align::Default, "Overlapped");
    craftingWindow->setVisible(false);
    craftingWindow->setCaption("Crafting window");
    craftingWindow->eventWindowButtonPressed += newDelegate(this, &GameHUD::OnCloseCraftingWindowClick);

    ListBox * list = craftingWindow->createWidget<ListBox>("ListBox", IntCoord(2, 2, 196, craftingWindow->getHeight() - 43), Align::Default, "recipes_list");
    list->eventListChangePosition += newDelegate(this, &GameHUD::OnSelectRecipe);

    Button * button = craftingWindow->createWidget<Button>("Button", IntCoord(206, craftingWindow->getHeight() - 63, craftingWindow->getWidth() - 15 - 186, 21), Align::Bottom);
    button->setCaption("CRAFT");
    button->eventMouseButtonClick += newDelegate(this, &GameHUD::OnCraftRequest);

    OnCraftingProgress();

    IntCoord imageCoords = { list->getLeft() + list->getWidth() + 8, 2, 64, 64 };
    craftingWindow->createWidget<ImageBox>("ImageBox", imageCoords, Align::Default, "item_image");

    IntCoord imageNameCoords = { 288 + 16, 2, 200, 32 };
    craftingWindow->createWidget<TextBox>("TextBox", imageNameCoords, Align::Default, "item_name")->setCaption("EMPTY NAME");

    IntCoord imageDescriptionCoords = { 288 + 16, 2 + 20, 200, 32 };
    craftingWindow->createWidget<TextBox>("TextBox", imageDescriptionCoords, Align::Default, "item_description")->setCaption("EMPTY DESC");

    int availableWidth = craftingWindow->getWidth() - 4 - list->getWidth() - 8; // 4 for borders and 8 for list margin
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            std::string name = "ingredient_icon_" + std::to_string(i * 2 + j);
            IntCoord imageWidgetCoords =
            {
                list->getLeft() + list->getWidth() + 8 + j * availableWidth / 2,
                72 + i * 70,
                64,
                64
            };
            craftingWindow->createWidget<ImageBox>("ImageBox", imageWidgetCoords, Align::Default, name);

            name = "ingredient_name_" + std::to_string(i * 2 + j);
            IntCoord nameWidgetCoords =
            {
                imageWidgetCoords.left + imageWidgetCoords.width + 4,
                imageWidgetCoords.top,
                availableWidth / 2 - imageWidgetCoords.width / 2 - 4,
                32
            };
            TextBox * t = craftingWindow->createWidget<TextBox>("TextBox", nameWidgetCoords, Align::Default, name);
            t->setCaption("NAME");

            name = "ingredient_count_" + std::to_string(i * 2 + j);
            IntCoord countWidgetCoords =
            {
                imageWidgetCoords.left + imageWidgetCoords.width + 4,
                imageWidgetCoords.top + 32,
                availableWidth / 2 - imageWidgetCoords.width / 2 - 4,
                32
            };
            t = craftingWindow->createWidget<TextBox>("TextBox", countWidgetCoords, Align::Default, name);
            t->setCaption("0");
        }
    }
}

void GameHUD::Update(double dt)
{
    auto pPlayer = LostIsland::GetGamePtr()->player;

    ProgressBar * p = guiModule->findWidget<ProgressBar>("health_bar");
    p->setProgressRange(pPlayer->GetMaxHealth());
    p->setProgressPosition(pPlayer->GetHealth());

    p = guiModule->findWidget<ProgressBar>("warmth_bar");
    p->setProgressRange(pPlayer->GetMaxWarmth());
    p->setProgressPosition(pPlayer->GetWarmth());
    Colour center = { 0.8f, 0.8f, 0.8f }, left = { 0.0f, 0.0f, 0.8f }, right = { 0.8f, 0.0f, 0.0f };
    float halfWarmth = pPlayer->GetMaxWarmth() / 2.0f;
    if (pPlayer->GetWarmth() > halfWarmth)
    {
        // hot
        Colour resultColor = {};
        resultColor.red = MathLib::LinearInterpolation(center.red, right.red, (float)(pPlayer->GetWarmth() - halfWarmth) / halfWarmth);
        resultColor.green = MathLib::LinearInterpolation(center.green, right.green, (float)(pPlayer->GetWarmth() - halfWarmth) / halfWarmth);
        resultColor.blue = MathLib::LinearInterpolation(center.blue, right.blue, (float)(pPlayer->GetWarmth() - halfWarmth) / halfWarmth);
        p->setColour(resultColor);
    }
    else
    {
        // cold
        Colour resultColor = {};
        resultColor.red = MathLib::LinearInterpolation(left.red, center.red, (float)(pPlayer->GetWarmth() - halfWarmth) / halfWarmth);
        resultColor.green = MathLib::LinearInterpolation(left.green, center.green, (float)(pPlayer->GetWarmth() - halfWarmth) / halfWarmth);
        resultColor.blue = MathLib::LinearInterpolation(left.blue, center.blue, (float)(pPlayer->GetWarmth() - halfWarmth) / halfWarmth);
        p->setColour(resultColor);
    }

    p = guiModule->findWidget<ProgressBar>("hunger_bar");
    p->setProgressRange(pPlayer->GetMaxHunger());
    p->setProgressPosition(pPlayer->GetHunger());
}
