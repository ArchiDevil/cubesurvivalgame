#include "gameHud.h"

#include "game.h"
#include "Entities/GameObjectInventory.h"
#include "Entities/PlayerGameObject.h"
#include "Items/Item.h"
#include "CraftingManager.h"

#include <GraphicsEngine/ShiftEngine.h>

using ShiftEngine::Sprite;

GameHUD::GameHUD()
{
    pIngredientIcons.fill(nullptr);
    pIngredientNames.fill(nullptr);
    pIngredientCounts.fill(nullptr);
}

void GameHUD::Initialize(SimpleGUI::Canvas * pCanvas)
{
    auto settings = ShiftEngine::GetContextManager()->GetEngineSettings();
    int screenWidth = settings.screenWidth;
    int screenHeight = settings.screenHeight;

    this->pCanvas = pCanvas;

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

    // not used now
    pInventory = new SimpleGUI::Window(pCanvas);
    pInventory->SetDraggable(false);
    pInventory->Hide();

    // creating crafting window
    pCrafting = new SimpleGUI::Window(pCanvas);
    pCrafting->SetSize(600, 300);
    pCrafting->SetPosition(300, 300);
    pCrafting->SetDraggable(false);
    pCrafting->Hide();

    SimpleGUI::Button * pCraftButton = new SimpleGUI::Button(pCrafting);
    pCraftButton->SetPosition(2, 275);
    pCraftButton->SetSize(596, 24);
    pCraftButton->SetText("CRAFT");
    pCraftButton->SetClickHandler([&](MouseKeys, int, int) { onCraftRequest(); });

    pRecipesList = new SimpleGUI::List(pCrafting);
    pRecipesList->SetPosition(2, 24);
    pRecipesList->SetSize(196, 250);
    pRecipesList->GetTable()->SetOnRowSelectedHandler([this](size_t row) { onSelectRecipe(row); });
    OnCraftingProgress();

    pItemImage = new SimpleGUI::Image(pCrafting, L"");
    pItemImage->SetPosition(224, 24);
    pItemImage->SetSize(64, 64);

    pItemName = new SimpleGUI::Text(pCrafting, "EMPTY NAME");
    pItemName->SetPosition(288 + 16, 24); // DNIWE, create layouts!
    pItemName->SetSize(100, 32);

    pItemDescription = new SimpleGUI::Text(pCrafting, "EMPTY DESCRIPTION");
    pItemDescription->SetPosition(288 + 16, 24 + 20);
    pItemDescription->SetSize(100, 32);

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            SimpleGUI::Image * pImage = new SimpleGUI::Image(pCrafting, L"");
            pImage->SetPosition(224 + j * 124, 94 + i * 70);
            pImage->SetSize(64, 64);
            pIngredientIcons[i * 2 + j] = pImage;

            SimpleGUI::Text * pName = new SimpleGUI::Text(pCrafting, "EMPTY");
            pName->SetPosition(224 + j * 124 + 64, 94 + i * 70);
            pName->SetSize(60, 32);
            pIngredientNames[i * 2 + j] = pName;

            SimpleGUI::Text * pCount = new SimpleGUI::Text(pCrafting, "0");
            pCount->SetPosition(224 + j * 124 + 64, 94 + i * 70 + 32);
            pCount->SetSize(60, 32);
            pIngredientCounts[i * 2 + j] = pCount;
        }
    }
}

void GameHUD::Draw()
{
    ShiftEngine::D3D10ContextManager * pCtxMgr = ShiftEngine::GetContextManager();
    auto settings = pCtxMgr->GetEngineSettings();
    int screenWidth = settings.screenWidth;
    int screenHeight = settings.screenHeight;

    auto pGame = LostIsland::GetGamePtr();
    auto pPlayer = pGame->Player;
    SlotUnit handItem = pPlayer->GetInventoryPtr()->GetItemInRightHand();

    for (int i = 0; i < 3; i++)
    {
        Vector2F position = Vector2F(8 + progressBarBack->GetTextureDimensions().x / 2, screenHeight - progressBarBack->GetTextureDimensions().y + 8 - i * 40);
        progressBarBack->SetPosition(position);
        progressBarBack->Draw();
    }

    float scale = (float)pGame->Player->GetHealth() / 20.0f;
    float xPos = 10 + healthBar->GetTextureDimensions().x * scale / 2;
    float yPos = screenHeight - progressBarBack->GetTextureDimensions().y + 8 - 2 * 40;
    healthBar->SetScale(Vector2F(scale, 1.0f));
    healthBar->SetPosition(Vector2F(xPos, yPos));

    scale = (float)pGame->Player->GetWarmth() / 100.0f;
    xPos = 10 + warmthBar->GetTextureDimensions().x * scale / 2;
    yPos = screenHeight - progressBarBack->GetTextureDimensions().y + 8 - 1 * 40;
    warmthBar->SetScale(Vector2F(scale, 1.0f));
    warmthBar->SetPosition(Vector2F(xPos, yPos));

    scale = (float)pGame->Player->GetHunger() / 25.0f;
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
    auto & pItemMgr = pGame->ItemMgr;

    for (int i = 0; i < 10; ++i)
    {
        itemsSprites[i]->SetTexture(ShiftEngine::ITexturePtr(nullptr));
    }

    const std::vector<SlotUnit> &items = pGame->Player->GetInventoryPtr()->GetItems();
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
    pGame->Player->GetInventoryPtr()->RemoveItemFromRightHand();
}

uint32_t GameHUD::GetSelectedSlot() const
{
    return selectedSlot;
}

void GameHUD::OpenCraftingWindow()
{
    pCrafting->Show();
}

void GameHUD::CloseCraftingWindow()
{
    pCrafting->Hide();
}

void GameHUD::OpenInventoryWindow()
{
    pInventory->Show();
}

void GameHUD::CloseInventoryWindow()
{
    pInventory->Hide();
}

void GameHUD::OnCraftingProgress()
{
    SimpleGUI::Table * pTable = pRecipesList->GetTable();
    CraftingManager * pCraftingMgr = LostIsland::GetGamePtr()->CratingMgr.get();
    auto recipesVec = pCraftingMgr->GetAllRecipes();
    SimpleGUI::ListModel * pModel = pTable->GetModel();
    if (!pModel)
        return;

    auto & pItemMgr = LostIsland::GetGamePtr()->ItemMgr;

    pModel->Clear();
    for (auto& recipe : recipesVec)
    {
        Item * pItem = pItemMgr->GetItemById(recipe.second.producedItem);
        if (!pItem)
            continue;

        SimpleGUI::ListModelElement elementToInsert(pItem->GetName());
        elementToInsert.SetValue("id", recipe.first);
        pModel->Add(elementToInsert);
    }
}

void GameHUD::onCraftRequest()
{
    SimpleGUI::Table * pTable = pRecipesList->GetTable();
    auto * pGame = LostIsland::GetGamePtr();

    size_t selectedRecipe = pTable->GetSelectedRow();
    if (selectedRecipe == ~0x0)
        return;

    SimpleGUI::ListModel * pModel = pTable->GetModel();
    if (!pModel)
        return;

    const std::string & name = pModel->Get(selectedRecipe).GetValue("id");
    const Recipe & recipe = pGame->CratingMgr->GetRecipeByName(name);
    pGame->CratingMgr->Craft(recipe);
    OnUserInventoryChange();
}

void GameHUD::onSelectRecipe(size_t row)
{
    // update icon, name and description
    SimpleGUI::ListModel * pModel = pRecipesList->GetTable()->GetModel();
    const std::string & recipeName = pModel->Get(row).GetValue("id");

    auto &pCraftingMgr = LostIsland::GetGamePtr()->CratingMgr;
    const Recipe &recipe = pCraftingMgr->GetRecipeByName(recipeName);

    auto &pItemMgr = LostIsland::GetGamePtr()->ItemMgr;
    Item * pItem = pItemMgr->GetItemById(recipe.producedItem);
    if (!pItem)
        LOG_ERROR("Unable to find item: ", recipe.producedItem);

    pItemImage->SetImage(utils::Widen(pItem->GetImageFile()));
    pItemName->SetText(pItem->GetName());
    pItemDescription->SetText(pItem->GetDescription());

    for (size_t i = 0; i < recipe.itemsToCraft.size(); ++i)
    {
        pIngredientIcons[i]->SetImage(L"");
        pIngredientNames[i]->SetText("");
        pIngredientCounts[i]->SetText("");
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

        pIngredientIcons[i]->SetImage(utils::Widen(pItem->GetImageFile()));
        pIngredientNames[i]->SetText(pItem->GetName());
        pIngredientCounts[i]->SetText(std::to_string(pair.second));
    }
}
