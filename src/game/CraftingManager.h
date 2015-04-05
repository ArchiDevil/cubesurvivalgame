#pragma once

#include <Utilities/IManager.h>

#include "Common.h"

#include <unordered_map>
#include <map>
#include <cstdint>
#include <array>

struct Recipe
{
    item_id_t producedItem;
    uint32_t producedCount;
    std::array<std::pair<item_id_t, uint32_t>, 4> itemsToCraft;
};

class CraftingManager : public IManager
{
public:
    CraftingManager();
    ~CraftingManager();

    void Initialize(const std::string & path);
    const Recipe& GetRecipeByName(const std::string & name) const;
    const Recipe& GetRecipeById(recipe_id_t id) const;
    std::vector<std::pair<std::string, Recipe>> GetAllRecipes() const;
    void Craft(const Recipe& recipe);

private:
    void LoadRecipes(const std::string & path);

    std::map<recipe_id_t, Recipe> HashRecipe;
    std::map<std::string, recipe_id_t> NameHash;
};
