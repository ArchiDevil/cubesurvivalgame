#pragma once

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

#include "../Common.h"

class Item;

class ItemManager
{
public:
    void Initialize(const std::string & PathName);
    Item * GetItemById(item_id_t itemId);
    item_id_t GetItemId(const std::string & ItemName);

private:
    void LoadDefinitions(const std::string & path);

    std::map<item_id_t, Item*> HashItem;
    std::map<std::string, item_id_t> NameHash;
};
