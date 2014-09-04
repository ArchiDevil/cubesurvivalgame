#pragma once

#include <string>

#include <GraphicsEngine/D3D10Texture.h>
#include <GraphicsEngine/cMesh.h>

class ItemUsingsHandler;

enum ItemType
{
	IT_Misc,
	IT_Weapon,
	IT_Entity,
	IT_Food,
	IT_Block
};

class Item
{
public:
	Item(ItemUsingsHandler * _handler, 
		  ShiftEngine::TexturePtr icon, 
		  ShiftEngine::MeshDataPtr mesh, 
		  const std::string & _Name, 
		  const std::string & _Desc);

	virtual ~Item();

	const std::string & GetDescription() const;
	const std::string & GetName() const;

	ShiftEngine::TexturePtr GetTexturePtr() const;
	ShiftEngine::MeshDataPtr GetMesh() const;
	
	virtual bool UseInWorld() = 0;
	virtual bool UseOnPlayer() = 0;
	virtual ItemType GetType() const = 0;

protected:
	ItemUsingsHandler * handler;
	const std::string name;
	const std::string description;
	const ShiftEngine::TexturePtr icon;
	ShiftEngine::MeshDataPtr mesh;

};
