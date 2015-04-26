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
    auto settings = ShiftEngine::GetContextManager()->GetEngineSettings();
    int screenWidth = settings.screenWidth;
    int screenHeight = settings.screenHeight;

    this->guiModule = guiModule;

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

    progressBarBack.reset(new Sprite(L"gui/backgroundbar.png"));
    Vector2F position = Vector2F(8 + progressBarBack->GetTextureDimensions().x / 2, screenHeight - progressBarBack->GetTextureDimensions().y + 8 - 2 * 40);
    healthBar.reset(new Sprite(L"gui/healthbar.png"));
    healthBar->SetPosition(position);
    position = Vector2F(8 + progressBarBack->GetTextureDimensions().x / 2, screenHeight - progressBarBack->GetTextureDimensions().y + 8 - 1 * 40);
    warmthBar.reset(new Sprite(L"gui/warmthbar.png"));
    warmthBar->SetPosition(position);
    hungerBar.reset(new Sprite(L"gui/hungerbar.png"));
    position = Vector2F(8 + progressBarBack->GetTextureDimensions().x / 2, screenHeight - progressBarBack->GetTextureDimensions().y + 8 - 0 * 40);
    hungerBar->SetPosition(position);

    //// not used now
    //pInventory = new SimpleGUI::Window(pCanvas);
    //pInventory->SetDraggable(false);
    //pInventory->Hide();

    craftingWindow = guiModule->createWidget<Window>("Window", IntCoord(300, 300, 600, 330), Align::Default, "Overlapped");
    craftingWindow->setMovable(false);
    craftingWindow->setVisible(false);
    craftingWindow->setCaption("Crafting window");

    Button * button = craftingWindow->createWidget<Button>("Button", IntCoord(2, 253, 596, 24), Align::Default);
    button->setCaption("CRAFT");
    button->eventMouseButtonClick += newDelegate(this, &GameHUD::OnCraftRequest);

    ListBox * list = craftingWindow->createWidget<ListBox>("ListBox", IntCoord(2, 2, 196, 250), Align::Default, "recipes_list");
    list->eventListChangePosition += newDelegate(this, &GameHUD::OnSelectRecipe);
    OnCraftingProgress();

    craftingWindow->createWidget<ImageBox>("ImageBox", IntCoord(224, 2, 64, 64), Align::Default, "item_image");
    craftingWindow->createWidget<TextBox>("TextBox", IntCoord(288 + 16, 2, 200, 32), Align::Default, "item_name")->setCaption("EMPTY NAME");
    craftingWindow->createWidget<TextBox>("TextBox", IntCoord(288 + 16, 2 + 20, 200, 32), Align::Default, "item_description")->setCaption("EMPTY DESC");

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            std::string name = "ingredient_icon_" + std::to_string(i) + std::to_string(j);
            craftingWindow->createWidget<ImageBox>("ImageBox", IntCoord(224 + j * 124, 72 + i * 70, 64, 64), Align::Default, name);

            name = "ingredient_name_" + std::to_string(i) + std::to_string(j);
            TextBox * t = craftingWindow->createWidget<TextBox>("TextBox", IntCoord(224 + j * 124 + 64, 72 + i * 70, 60, 32), Align::Default, name);
            t->setCaption("EMPTY NAME");

            name = "ingredient_count_" + std::to_string(i) + std::to_string(j);
            t = craftingWindow->createWidget<TextBox>("TextBox", IntCoord(224 + j * 124 + 64, 72 + i * 70 + 32, 60, 32), Align::Default, name);
            t->setCaption("0");
        }
    }
}

void GameHUD::Draw()
{
    ShiftEngine::IContextManager * pCtxMgr = ShiftEngine::GetContextManager();
    auto settings = pCtxMgr->GetEngineSettings();
    int screenWidth = settings.screenWidth;
    int screenHeight = settings.screenHeight;

    auto pGame = LostIsland::GetGamePtr();
    auto pPlayer = pGame->player;
    SlotUnit handItem = pPlayer->GetInventoryPtr()->GetItemInRightHand();

    for (int i = 0; i < 3; i++)
    {
        Vector2F position = Vector2F(8 + progressBarBack->GetTextureDimensions().x / 2, screenHeight - progressBarBack->GetTextureDimensions().y + 8 - i * 40);
        progressBarBack->SetPosition(position);
        progressBarBack->Draw();
    }

    float scale = (float)pGame->player->GetHealth() / 20.0f;
    float xPos = 10 + healthBar->GetTextureDimensions().x * scale / 2;
    float yPos = screenHeight - progressBarBack->GetTextureDimensions().y + 8 - 2 * 40;
    healthBar->SetScale(Vector2F(scale, 1.0f));
    healthBar->SetPosition(Vector2F(xPos, yPos));

    scale = (float)pGame->player->GetWarmth() / 100.0f;
    xPos = 10 + warmthBar->GetTextureDimensions().x * scale / 2;
    yPos = screenHeight - progressBarBack->GetTextureDimensions().y + 8 - 1 * 40;
    warmthBar->SetScale(Vector2F(scale, 1.0f));
    warmthBar->SetPosition(Vector2F(xPos, yPos));

    scale = (float)pGame->player->GetHunger() / 25.0f;
    xPos = 10 + hungerBar->GetTextureDimensions().x * scale / 2;
    yPos = screenHeight - progressBarBack->GetTextureDimensions().y + 8 - 0 * 40;
    hungerBar->SetScale(Vector2F(scale, 1.0f));
    hungerBar->SetPosition(Vector2F(xPos, yPos));

    healthBar->Draw();
    warmthBar->Draw();
    hungerBar->Draw();

    itemPanel->Draw();
    selectedBorder->SetPosition({ screenWidth / 2 - itemPanel->GetTextureDimensions().x / 2 + 24.0f + selectedSlot*48.0f, screenHeight - 30.0f });
    selectedBorder->Draw();

    const std::vector<SlotUnit> &items = pPlayer->GetInventoryPtr()->GetItems();
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
    auto pGame = LostIsland::GetGamePtr();
    pGame->player->GetInventoryPtr()->RemoveItemFromRightHand();
}

uint32_t GameHUD::GetSelectedSlot() const
{
    return selectedSlot;
}

void GameHUD::OpenCraftingWindow()
{
    if (craftingWindow)
        craftingWindow->setVisible(true);
}

void GameHUD::CloseCraftingWindow()
{
    if (craftingWindow)
        craftingWindow->setVisible(false);
}

void GameHUD::OpenInventoryWindow()
{
    //pInventory->Show();
}

void GameHUD::CloseInventoryWindow()
{
    //pInventory->Hide();
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
    pGame->craftingMgr->Craft(recipe);
    OnUserInventoryChange();
}

void GameHUD::OnSelectRecipe(Widget * _sender, size_t row)
{
    auto &pCraftingMgr = LostIsland::GetGamePtr()->craftingMgr;
    auto &pItemMgr = LostIsland::GetGamePtr()->itemMgr;

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

    for (size_t i = 0; i < recipe.itemsToCraft.size(); ++i)
    {
        itemImage = craftingWindow->findWidget("ingredient_icon_" + std::to_string(i / 2) + std::to_string((i % 2)))->castType<ImageBox>();
        itemImage->setImageTexture("");

        itemName = craftingWindow->findWidget("ingredient_name_" + std::to_string(i / 2) + std::to_string((i % 2)))->castType<TextBox>();
        itemName->setCaption("");

        itemName = craftingWindow->findWidget("ingredient_count_" + std::to_string(i / 2) + std::to_string((i % 2)))->castType<TextBox>();
        itemName->setCaption("");
    }

    for (size_t i = 0; i < recipe.itemsToCraft.size(); ++i)
    {
        auto &pair = recipe.itemsToCraft[i];
        if (!pair.first || !pair.second)
        {
            continue;
        }

        pItem = pItemMgr->GetItemById(pair.first);
        if (!pItem)
        {
            continue;
        }

        itemImage = craftingWindow->findWidget("ingredient_icon_" + std::to_string(i / 2) + std::to_string((i % 2)))->castType<ImageBox>();
        itemImage->setImageTexture(pItem->GetImageFile());

        itemName = craftingWindow->findWidget("ingredient_name_" + std::to_string(i / 2) + std::to_string((i % 2)))->castType<TextBox>();
        itemName->setCaption(pItem->GetName());

        itemName = craftingWindow->findWidget("ingredient_count_" + std::to_string(i / 2) + std::to_string((i % 2)))->castType<TextBox>();
        itemName->setCaption(std::to_string(pair.second));
    }
}
