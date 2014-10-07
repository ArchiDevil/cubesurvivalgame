#pragma once

#include <map>
#include <string>
#include <vector>
#include <fstream>

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
	uint64_t GetBlockItem(BlockType type);

	Item * GetItemById(uint64_t itemId);
	uint64_t GetItemId(const std::string & ItemName);

private:
	void LoadDefinitions(const std::wstring & path);
	void LoadBlocks();

	std::map<uint64_t, Item*> HashItem;
	std::map<std::string, uint64_t> NameHash;

	typesStorage * ts;
	ItemUsingsHandler handler;

};
