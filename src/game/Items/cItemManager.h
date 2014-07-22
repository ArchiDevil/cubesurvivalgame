#pragma once

#include <map>
#include <string>
#include <vector>
#include <fstream>

#include "../../utilities/ut.h"

#include "cItem.h"
#include "cBlockItem.h"
#include "cMiscItem.h"
#include "cArmorItem.h"
#include "cPotionItem.h"
#include "cWeaponItem.h"
#include "cToolItem.h"

#include "../world/typesStorage.h"
#include "../cItemUsingHandler.h"
#include "../world/world.h"

class cItem;

class cItemManager
{
public:
	cItemManager(cPlayer * _p, cWorld * _w, typesStorage * _ts);
	~cItemManager();

	void Initialize(const std::wstring & PathName);
	cItem * GetItemByName(const std::string & Name);
	cItem * GetBlockItem(BlockType type);

private:
	void LoadDefinitions(const std::wstring & path);
	void LoadItem(const std::wstring & filename);
	void LoadBlocks();

	std::map<std::string, cItem *> Items;
	std::vector<cItem *> Blocks;
	typesStorage * ts;
	cItemUsingsHandler handler;

};
