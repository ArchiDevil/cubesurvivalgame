#pragma once

#include <map>
#include <string>
#include <vector>
#include <fstream>

#include <utilities/ut.h>

#include "Item.h"
#include "BlockItem.h"
#include "MiscItem.h"
#include "ArmorItem.h"
#include "PotionItem.h"
#include "WeaponItem.h"
#include "ToolItem.h"

#include "../world/typesStorage.h"
#include "../ItemUsingHandler.h"
#include "../world/world.h"

class Item;

class ItemManager
{
public:
	ItemManager(cPlayer * _p, cWorld * _w, typesStorage * _ts);
	~ItemManager();

	void Initialize(const std::wstring & PathName);
	Item * GetBlockItem(BlockType type);

	Item * GetItemById(uint64_t itemId);
	uint64_t GetItemId(std::string & ItemName);

private:
	void LoadDefinitions(const std::wstring & path);
	void LoadBlocks();

	std::map<uint64_t, Item*> Items;

	std::vector<Item *> Blocks;
	typesStorage * ts;
	ItemUsingsHandler handler;

};
