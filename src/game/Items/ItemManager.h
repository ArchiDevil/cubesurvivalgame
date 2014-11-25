#pragma once

#include <map>
#include <string>
#include <vector>
#include <fstream>

#include "../world/typesStorage.h"
#include "../ItemUsingHandler.h"
#include "../world/world.h"

class Item;

typedef uint64_t item_id_t;

class ItemManager
{
public:
	ItemManager(PlayerGameObject * _p, cWorld * _w, typesStorage * _ts);
	~ItemManager();

	void Initialize(const std::wstring & PathName);
	Item * GetItemById(item_id_t itemId);
	item_id_t GetItemId(const std::string & ItemName);

private:
	void LoadDefinitions(const std::wstring & path);

	std::map<item_id_t, Item*> HashItem;
	std::map<std::string, item_id_t> NameHash;

	typesStorage * ts;
	ItemUsingsHandler handler;

};
