#pragma once

#include <string>

#include <GraphicsEngine/D3D10Texture.h>
#include <GraphicsEngine/cMesh.h>

class ItemUsingsHandler;

class Item
{
public:
	Item(ItemUsingsHandler * _handler, 
		  ShiftEngine::TexturePtr icon, 
		  ShiftEngine::MeshDataPtr mesh, 
		  const std::string & _Name, 
		  const std::string & _Desc);

	virtual ~Item();

	std::string GetDescription() const;
	std::string GetName() const;

	ShiftEngine::TexturePtr GetTexturePtr();
	ShiftEngine::MeshDataPtr * GetMesh();
	
	virtual bool UseInWorld() = 0;
	virtual bool UseOnPlayer() = 0;

protected:
	ItemUsingsHandler * handler;
	const ShiftEngine::TexturePtr icon;
	ShiftEngine::MeshDataPtr mesh;

	const std::string Description;
	const std::string Name;

};
