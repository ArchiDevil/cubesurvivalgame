#pragma once

#include "cMaterial.h"
#include "D3D10TextureManager.h"
#include "ShaderManager.h"

namespace tinyxml2
{
	class XMLElement;
}

namespace ShiftEngine
{
	class MaterialManager
	{
	public:
		MaterialManager(D3D10TextureManager * _manager, D3D10ShaderManager * _shaderManager);
		~MaterialManager();

		MaterialPtr LoadMaterial( const std::wstring & filename, const std::wstring & mtlName = L"default" );

	private:
		bool CheckMaterialName( const std::string & name );
		bool FillProperties( MaterialInfo & info, const std::map<std::string, tinyxml2::XMLElement*> & KeyVal );

		bool LoadMaps( MaterialInfo & info, const std::map<std::string, tinyxml2::XMLElement*> & KeyVal );
		bool LoadColors( MaterialInfo & info, const std::map<std::string, tinyxml2::XMLElement*> & KeyVal );
		bool LoadOtherFlags( MaterialInfo & info, const std::map<std::string, tinyxml2::XMLElement*> & KeyVal );
		bool LoadTextures( MaterialInfo & info, MaterialPtr & ptr );

		std::vector<MaterialPtr> materials;

		D3D10TextureManager * pTextureManager;
		D3D10ShaderManager * pShaderManager;

	};
}