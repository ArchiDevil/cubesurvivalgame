#include "cItemManager.h"

#include <cassert>

#include "../../GraphicsEngine/ShiftEngine.h"

cItemManager::cItemManager( cPlayer * _p, cWorld * _w, typesStorage * _ts )
	: handler(_p, _w), ts(_ts) {}

cItemManager::~cItemManager() {}

void cItemManager::Initialize( const std::wstring & PathName )
{
	LoadDefinitions(PathName);
	LoadBlocks();
}

void cItemManager::LoadDefinitions( const std::wstring & path )
{
	Items["empty"] = new cMiscItem(nullptr, "null", "null", nullptr, nullptr);	//empty item

	std::wstring str = path;

	auto filenames = ::utils::CollectFileNames(path, L"itm");
	for(auto iter = filenames.begin(); iter != filenames.end(); iter++)
		LoadItem(str + *iter);
}

void cItemManager::LoadItem( const std::wstring & filename )
{
	//enum types
	//{
	//	WEAPON = 1,
	//	ARMOR,
	//	POTION,
	//	TOOL,
	//	MISC,
	//};

 //	std::ifstream in(filename);
 //	std::string name, desc, textureName, meshName;
 //	std::getline(in, name);
 //	std::getline(in, desc);
	//std::getline(in, textureName);
	//std::getline(in, meshName);
	//MeshDataPtr mesh = cGraphicsEngine::GetInstance().LoadMesh(utils::StrToWStr(meshName)).GetData();
	//cTexturePtr texture = cGraphicsEngine::GetInstance().LoadTexture(utils::StrToWStr(textureName));
	////solve type
	//std::string buffer;
	//std::getline(in, buffer);
	//types type = types(atoi(buffer.c_str()));

	//if(type == WEAPON)
	//{
	//	std::getline(in, buffer);
	//	int damage = atoi(buffer.c_str());	//damage points

	//	cItem * item = new cWeaponItem(&handler, name, desc, mesh, texture, damage);
	//	Items[name] = item;
	//}
	//else if (type == ARMOR)
	//{
	//	std::getline(in, buffer);
	//	bodyParts bPart = (bodyParts)atoi(buffer.c_str());	//body part

	//	std::getline(in, buffer);
	//	int ap = atoi(buffer.c_str());	//armor points

	//	std::getline(in, buffer);
	//	int wp = atoi(buffer.c_str());	//warm points

	//	cItem * item = new cArmorItem(&handler, name, desc, mesh, texture, bPart, ap, wp);
	//	Items[name] = item;
	//}
	//else if (type == POTION)
	//{
	//	std::getline(in, buffer);
	//	int hp = atoi(buffer.c_str());	//health points

	//	std::getline(in, buffer);
	//	int fp = atoi(buffer.c_str());	//fatique point

	//	std::getline(in, buffer);
	//	int hnp = atoi(buffer.c_str());	//hunger points

	//	std::getline(in, buffer);
	//	int hd = atoi(buffer.c_str());	//health damage

	//	cItem * item = new cPotionItem(&handler, name, desc, texture, mesh, hp, fp, hnp, hd);
	//	Items[name] = item;
	//}
	//else if (type == TOOL)
	//{
	//	cItem * item = new cToolItem(&handler, name, desc, mesh, texture);
	//	Items[name] = item;
	//}
	//else if (type == MISC)
	//{
	//	cItem * item = new cMiscItem(&handler, name, desc, mesh, texture);
	//	Items[name] = item;
	//}
	//else
	//{
	//	assert("something wrong?");
	//}
}

void cItemManager::LoadBlocks()
{
	static_assert(ID_END == 7U, "Update loop!");
	std::vector<std::wstring> textures = ts->getTexturesNames();

	cItem * blockAir = new cBlockItem(&handler, "0", "0", nullptr, ID_AIR);
	Blocks.push_back(blockAir);

	assert(ShiftEngine::GetContextManager() != 0);

	for (int index = (int)ID_WROOT; index <= (int)ID_SAND; index++)
	{
		cItem * block = new cBlockItem(&handler, std::to_string(index), std::to_string(index), 
			ShiftEngine::GetContextManager()->LoadTexture(textures[index - 1]), (BlockType)index);
		Blocks.push_back(block);
	}
}

cItem * cItemManager::GetItemByName( const std::string & Name )
{
	return Items[Name];
}

cItem * cItemManager::GetBlockItem( BlockType type )
{
	return Blocks[(int)type];
}
