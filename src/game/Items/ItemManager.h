#pragma once

#include <map>
#include <string>
#include <vector>
#include <fstream>

#include "../../utilities/ut.h"

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
	Item * GetItemByName(const std::string & Name);
	Item * GetBlockItem(BlockType type);

private:
	void LoadDefinitions(const std::wstring & path);
	void LoadItem(const std::wstring & filename);
	void LoadBlocks();

	std::map<std::string, Item *> Items;
	std::vector<Item *> Blocks;
	typesStorage * ts;
	ItemUsingsHandler handler;

};
