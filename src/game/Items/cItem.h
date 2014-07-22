#pragma once

#include <string>

#include "../../GraphicsEngine/D3D10Texture.h"
#include "../../GraphicsEngine/cMesh.h"
#include "../cItemUsingHandler.h"

class cItemUsingsHandler;

class cItem
{
public:
	cItem(cItemUsingsHandler * _handler, 
		  ShiftEngine::TexturePtr ptr, 
		  ShiftEngine::MeshDataPtr data, 
		  const std::string & _Name = "NULL", 
		  const std::string & _Desc = "NULL");

	virtual ~cItem();

	std::string GetDescription() const;
	std::string GetName() const;

	int GetDurability() const;
	void SetDurability(int _durability);

	int GetQuality() const { return quality; }
	void SetQuality(int val) { quality = val; }

	ShiftEngine::TexturePtr GetTexturePtr();
	ShiftEngine::MeshDataPtr * GetMesh();
	
	virtual bool Use() = 0;

protected:
	cItemUsingsHandler * handler;
	const ShiftEngine::TexturePtr texture;
	ShiftEngine::MeshDataPtr mesh;

private:
	const std::string Description;
	const std::string Name;
	int durability;
	int quality;

	//does it need cost?
};