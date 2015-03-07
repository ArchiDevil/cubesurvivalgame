#include "gameHud.h"

#include "game.h"
#include "Entities/GameObjectInventory.h"
#include "Entities/PlayerGameObject.h"
#include "Items/Item.h"
#include "CraftingManager.h"

#include <GraphicsEngine/ShiftEngine.h>

using ShiftEngine::Sprite;

gameHUD::gameHUD()
{
}

void gameHUD::Initialize(SimpleGUI::Canvas * pCanvas)
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

    pCrafting = new SimpleGUI::Window(pCanvas);
    pCrafting->Hide();
    pCrafting->SetSize(600, 300);
    pCrafting->SetPosition(300, 300);
    pCrafting->SetDraggable(false);

    SimpleGUI::Button * pCraftButton = new SimpleGUI::Button(pCrafting);
    pCraftButton->SetPosition(2, 275);
    pCraftButton->SetSize(596, 24);
    pCraftButton->SetText("CRAFT");
    pCraftButton->SetClickHandler([&](MouseKeys, int, int){ onCraftRequest(); });

    pRecipesList = new SimpleGUI::List(pCrafting);
    pRecipesList->SetSize(196, 250);
    pRecipesList->SetPosition(2, 24);
    OnCraftingProgress();
}

void gameHUD::Draw()
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

void gameHUD::OnUserInventoryChange()
{
    auto pGame = LostIsland::GetGamePtr();
    auto & pItemMgr = pGame->ItemMgr;

    for (int i = 0; i < 10; ++i)
    {
        itemsSprites[i]->SetTexture(ShiftEngine::TexturePtr(nullptr));
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

void gameHUD::SelectSlot(uint32_t slot)
{
    selectedSlot = slot;
    auto pGame = LostIsland::GetGamePtr();
    pGame->Player->GetInventoryPtr()->RemoveItemFromRightHand();
}

uint32_t gameHUD::GetSelectedSlot() const
{
    return selectedSlot;
}

void gameHUD::OpenCraftingWindow()
{
    pCrafting->Show();
}

void gameHUD::CloseCraftingWindow()
{
    pCrafting->Hide();
}

void gameHUD::OpenInventoryWindow()
{
    pInventory->Show();
}

void gameHUD::CloseInventoryWindow()
{
    pInventory->Hide();
}

void gameHUD::OnCraftingProgress()
{
    auto * pTable = pRecipesList->GetTable();
    auto * pCraftingMgr = LostIsland::GetGamePtr()->CratingMgr.get();
    auto recipesVec = pCraftingMgr->GetAllRecipes();
    pTable->Clear();
    for (auto & recipe : recipesVec)
    {
        pTable->AddRow(recipe.first);
    }
}

void gameHUD::onCraftRequest()
{
    SimpleGUI::TableRow * selectedRecipe = pRecipesList->GetTable()->GetSelectedRow();
    if (!selectedRecipe)
        return;

    auto str = selectedRecipe->GetString();
    auto * pGame = LostIsland::GetGamePtr();
    const Recipe & recipe = pGame->CratingMgr->GetRecipeByName(str);
    pGame->CratingMgr->Craft(recipe);
    OnUserInventoryChange();
}
