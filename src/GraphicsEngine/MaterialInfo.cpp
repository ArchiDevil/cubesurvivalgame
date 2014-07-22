#include "MaterialInfo.h"

ShiftEngine::materialFlags::materialFlags() 
	: useVertexColors(false), isCustom(false), 
	isReflective(false), isTransparent(false), 
	twoFace(false),	
	lightingModel(MaterialLightingModel::Phong)
{
}

bool ShiftEngine::materialFlags::operator==( const materialFlags & ref ) const
{
	return useVertexColors == ref.useVertexColors
		&& lightingModel == ref.lightingModel
		&& twoFace == ref.twoFace
		&& isReflective == ref.isReflective
		&& isTransparent == ref.isTransparent
		&& isCustom == ref.isCustom;
}

//////////////////////////////////////////////////////////////////////////

ShiftEngine::MaterialInfo::MaterialInfo() 
	: diffuseColor(1.0f, 1.0f, 1.0f, 1.0f), specularColor(1.0f, 1.0f, 1.0f, 1.0f), 
	emissionColor(0.0f, 0.0f, 0.0f, 0.0f), opacity(1.0f), shininess(25.0f)
{
}

ShiftEngine::MaterialInfo::MaterialInfo( const materialFlags & _flags ) 
	: diffuseColor(1.0f, 1.0f, 1.0f, 1.0f), specularColor(1.0f, 1.0f, 1.0f, 1.0f),
	emissionColor(0.0f, 0.0f, 0.0f, 0.0f), opacity(1.0f),
	shininess(25.0f), flags(_flags)
{

}

const ShiftEngine::materialFlags * ShiftEngine::MaterialInfo::GetFlags() const
{
	return &flags;
}

bool ShiftEngine::MaterialInfo::operator<( const MaterialInfo & ref ) const
{
	if(*this == ref)
		return false;

	std::hash<std::string> stringHash;

	//std::string str = diffuseMap + normalMap + specularMap + alphaMap + environmentMap;
	size_t curHash;// = stringHash(str);

	//str = ref.diffuseMap + ref.normalMap + ref.specularMap + ref.alphaMap + ref.environmentMap;
	size_t refHash;// = stringHash(str);

	//if(curHash != refHash)
	//	return curHash < refHash;

	std::string curBytes;
	uint8_t * start = (uint8_t*)this;
	uint8_t * end = (uint8_t*)&diffuseMap;
	while(start != end)
	{
		curBytes.push_back(*start);
		start++;
	}
	curHash = stringHash(curBytes);

	curBytes.clear();
	start = (uint8_t*)&ref;
	end = (uint8_t*)&ref.diffuseMap;
	while(start != end)
	{
		curBytes.push_back(*start);
		start++;
	}
	refHash = stringHash(curBytes);

	return curHash < refHash;
}

bool ShiftEngine::MaterialInfo::operator==( const MaterialInfo & ref ) const
{
	return flags == ref.flags
		&& diffuseColor == ref.diffuseColor
		&& specularColor == ref.specularColor
		&& emissionColor == ref.emissionColor
		&& diffuseMap == ref.diffuseMap
		//&& normalMap == ref.normalMap
		//&& specularMap == ref.specularMap
		//&& environmentMap == ref.environmentMap
		&& alphaMap == ref.alphaMap
		&& opacity == ref.opacity
		&& shininess == ref.shininess;
}

//////////////////////////////////////////////////////////////////////////

ShiftEngine::MaterialTextureDescription::MaterialTextureDescription( TextureType _type ) 
	: type(_type)
{
}

ShiftEngine::TextureType ShiftEngine::MaterialTextureDescription::GetType() const
{
	return type;
}

const std::vector<std::string> & ShiftEngine::MaterialTextureDescription::GetMaps() const
{
	return maps;
}

bool ShiftEngine::MaterialTextureDescription::operator==( const MaterialTextureDescription & ref ) const
{
	if(this->type != ref.type)
		return false;

	if(this->maps.size() != ref.maps.size())
		return false;

	for(unsigned int i = 0; i < maps.size(); i++)
		if(this->maps[i] != ref.maps[i])
			return false;

	return true;
}

void ShiftEngine::MaterialTextureDescription::SetMaps( const std::vector<std::string> & maps )
{
	this->maps = maps;
}
