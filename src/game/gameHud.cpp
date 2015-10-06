#include "gameHud.h"

#include "game.h"
#include "Entities/GameObjectInventory.h"
#include "Entities/PlayerGameObject.h"
#include "Items/Item.h"
#include "CraftingManager.h"

#include <GraphicsEngine/ShiftEngine.h>

using ShiftEngine::Sprite;
using namespace MyGUI;

static const char slotNamePrefix[] = "slot_";

#if _MSC_VER < 1900
    #define constexpr
#endif

template<typename T, size_t N>
constexpr size_t countof(T(&)[N])
{
    return N;
}

GameHUD::GameHUD(MyGUI::Gui * guiModule)
    : guiModule(guiModule)
{
    CreateInventoryWindow();
    CreateCraftingWindow();
    CreateOtherElements();
}

GameHUD::~GameHUD()
{
}

void GameHUD::Draw()
{
}

void GameHUD::ReloadInventory()
{
    auto pGame = LostIsland::GetGamePtr();
    auto & pItemMgr = pGame->itemMgr;

    const std::vector<SlotUnit> &items = pGame->player->GetInventoryPtr()->GetItems();

    int widthMargin = 5;
    for (size_t i = 0; i < items.size(); ++i)
    {
        auto item = pItemMgr->GetItemById(items[i].itemId);
        if (!item || !items[i].count)
            continue;

        ImageBox * pImage = inventoryWindow->findWidget(slotNamePrefix + std::to_string(i))->castType<ImageBox>();
        pImage->setImageTexture(item->GetImageFile());

        TextBox * pTextBox = pImage->findWidget("count")->castType<TextBox>();
        pTextBox->setCaption(std::to_string(items[i].count));
        IntSize textSize = pTextBox->getTextSize();
        textSize.width += widthMargin;
        IntSize imageSize = pImage->getSize();
        pTextBox->setPosition({ imageSize.width - textSize.width, imageSize.height - textSize.height });
        pTextBox->setSize(textSize);
    }
}

void GameHUD::SwitchInventoryWindow()
{
    if (!inventoryWindow)
        return;

    ReloadInventory();
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
    ReloadInventory();
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
            ingredientName->setTextColour({ 1.0f, 0.0f, 0.0f });
            ingredientCount->setTextColour({ 1.0f, 0.0f, 0.0f });
        }
        else
        {
            ingredientImage->setAlpha(1.0f);
            ingredientName->setTextColour({ 0.0f, 0.0f, 0.0f });
            ingredientCount->setTextColour({ 0.0f, 0.0f, 0.0f });
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

void GameHUD::OnCloseInventoryWindowClick(MyGUI::Widget * _sender, const std::string & eventName)
{
    inventoryWindow->setVisible(false);
}

void GameHUD::OnInventoryImageClick(MyGUI::Widget * _sender)
{
    if (!_sender)
        return;

    constexpr int size = countof(slotNamePrefix);
    auto pPlayer = LostIsland::GetGamePtr()->player;

    int slotIndex = 0;
    std::string name = _sender->getName();
    if (name.empty())
        return;

    std::string slotString = name.substr(size - 1);
    slotIndex = std::stoi(slotString);
    pPlayer->GetInventoryPtr()->SetItemIntoRightHand(slotIndex);

    item_id_t itemId = pPlayer->GetInventoryPtr()->GetItemInSlot(slotIndex).itemId;
    Item * pItem = LostIsland::GetGamePtr()->itemMgr->GetItemById(itemId);
    if (!pItem)
        return;

    rightHandImage->setImageTexture(pItem->GetImageFile());
}

void GameHUD::CreateOtherElements()
{
    auto settings = ShiftEngine::GetContextManager()->GetEngineSettings();
    int screenWidth = settings.screenWidth;
    int screenHeight = settings.screenHeight;

    const char * names[] = { "health_bar", "warmth_bar", "hunger_bar" };
    const Colour colors[] = { { 1.0f, 0.0f, 0.0f }, { 0.8f, 0.8f, 0.8f }, { 0.8f, 0.8f, 0.0f } };

    for (int i = 0; i < 3; ++i)
    {
        ProgressBar * bar = guiModule->createWidget<ProgressBar>("ProgressBarFill",
        { 8, screenHeight - 90 + i * 30, 200, 24 },
                                                                 Align::Default,
                                                                 "Overlapped",
                                                                 names[i]);

        bar->setProgressRange(100);
        bar->setProgressPosition(50);
        bar->setColour(colors[i]);
    }

    const int panelSize = 76;
    const int panelMargin = 5;
    IntCoord itemCoords
    {
        screenWidth - panelSize - panelMargin, 
        screenHeight - panelSize - panelMargin, 
        panelSize, 
        panelSize
    };

    rightHandPanel = guiModule->createWidget<Widget>("ItemBoxNoScroll", itemCoords, Align::Default, "Overlapped");
    rightHandImage = rightHandPanel->createWidget<ImageBox>("ImageBox", { 5, 5, 64, 64 }, Align::Default);

}

void GameHUD::CreateInventoryWindow()
{
    const int imageMargin = 10;
    const int imageSize = 64;
    const int stride = imageSize + imageMargin;
    const int rows = 4;
    const int columns = 4;
    const int leftPanelWidth = 0; // for now, it has 0 width, cause we render it without params

    IntCoord windowCoords
    {
        100,                                                 // x
        100,                                                 // y
        leftPanelWidth + columns * stride + imageMargin * 2, // left panel + margin + count of columns + margin
        rows * stride + 22 + imageMargin * 2                 // margin + count of rows + windows header width (22 pixels) + margin
    };

    inventoryWindow = guiModule->createWidget<Window>("WindowCX", windowCoords, Align::Default, "Overlapped");
    inventoryWindow->setVisible(false);
    inventoryWindow->setCaption("Inventory window");
    inventoryWindow->eventWindowButtonPressed += newDelegate(this, &GameHUD::OnCloseInventoryWindowClick);

    for (int row = 0; row < rows; ++row)
    {
        for (int column = 0; column < columns; ++column)
        {
            std::string slotName = slotNamePrefix + std::to_string(row * 4 + column);
            int startX = leftPanelWidth + imageMargin + stride * column;
            int startY = imageMargin + stride * row;

            ImageBox * pImage = inventoryWindow->createWidget<ImageBox>("ImageBox", { startX, startY, imageSize, imageSize }, Align::Default, slotName);
            pImage->eventMouseButtonClick += newDelegate(this, &GameHUD::OnInventoryImageClick);
            // no coords and no size for creation, they will be updated on inventory change callback
            TextBox * pTextBox = pImage->createWidget<TextBox>("TextBox", {}, Align::Right | Align::Bottom, "count");
        }
    }
}

void GameHUD::CreateCraftingWindow()
{
    craftingWindow = guiModule->createWidget<Window>("WindowCX", { 100, 100, 600, 330 }, Align::Default, "Overlapped");
    craftingWindow->setVisible(false);
    craftingWindow->setCaption("Crafting window");
    craftingWindow->eventWindowButtonPressed += newDelegate(this, &GameHUD::OnCloseCraftingWindowClick);

    ListBox * recipes_list = craftingWindow->createWidget<ListBox>("ListBox", { 2, 2, 196, craftingWindow->getHeight() - 43 }, Align::Default, "recipes_list");
    recipes_list->eventListChangePosition += newDelegate(this, &GameHUD::OnSelectRecipe);

    Button * craft_button = craftingWindow->createWidget<Button>("Button", { 206, craftingWindow->getHeight() - 63, craftingWindow->getWidth() - 15 - 186, 21 }, Align::Bottom);
    craft_button->setCaption("CRAFT");
    craft_button->eventMouseButtonClick += newDelegate(this, &GameHUD::OnCraftRequest);

    OnCraftingProgress();

    IntCoord imageCoords{ recipes_list->getLeft() + recipes_list->getWidth() + 8, 2, 64, 64 };
    craftingWindow->createWidget<ImageBox>("ImageBox", imageCoords, Align::Default, "item_image");

    IntCoord imageNameCoords{ 288 + 16, 2, 200, 32 };
    craftingWindow->createWidget<TextBox>("TextBox", imageNameCoords, Align::Default, "item_name")->setCaption("EMPTY NAME");

    IntCoord imageDescriptionCoords{ 288 + 16, 2 + 20, 200, 32 };
    craftingWindow->createWidget<TextBox>("TextBox", imageDescriptionCoords, Align::Default, "item_description")->setCaption("EMPTY DESC");

    int availableWidth = craftingWindow->getWidth() - 4 - recipes_list->getWidth() - 8; // 4 for borders and 8 for list margin
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            std::string name = "ingredient_icon_" + std::to_string(i * 2 + j);
            IntCoord imageWidgetCoords
            {
                recipes_list->getLeft() + recipes_list->getWidth() + 8 + j * availableWidth / 2,
                72 + i * 70,
                64,
                64
            };
            craftingWindow->createWidget<ImageBox>("ImageBox", imageWidgetCoords, Align::Default, name);

            name = "ingredient_name_" + std::to_string(i * 2 + j);
            IntCoord nameWidgetCoords
            {
                imageWidgetCoords.left + imageWidgetCoords.width + 4,
                imageWidgetCoords.top,
                availableWidth / 2 - imageWidgetCoords.width / 2 - 4,
                32
            };
            TextBox * t = craftingWindow->createWidget<TextBox>("TextBox", nameWidgetCoords, Align::Default, name);
            t->setCaption("NAME");

            name = "ingredient_count_" + std::to_string(i * 2 + j);
            IntCoord countWidgetCoords
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
