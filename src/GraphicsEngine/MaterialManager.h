#pragma once

#include "Material.h"
#include "ITextureManager.h"
#include "IShaderManager.h"

namespace tinyxml2
{
    class XMLElement;
}

namespace ShiftEngine
{
    class MaterialManager
    {
    public:
        MaterialManager(ITextureManager * _manager, IShaderManager * _shaderManager);
        ~MaterialManager();

        MaterialPtr LoadMaterial(const std::wstring & filename, const std::wstring & mtlName = L"default");

    private:
        bool CheckMaterialName(const std::string & name);
        bool FillProperties(MaterialInfo & info, const std::map<std::string, tinyxml2::XMLElement*> & KeyVal);

        bool LoadMaps(MaterialInfo & info, const std::map<std::string, tinyxml2::XMLElement*> & KeyVal);
        bool LoadColors(MaterialInfo & info, const std::map<std::string, tinyxml2::XMLElement*> & KeyVal);
        bool LoadOtherFlags(MaterialInfo & info, const std::map<std::string, tinyxml2::XMLElement*> & KeyVal);
        bool LoadTextures(MaterialInfo & info, MaterialPtr & ptr);

        std::vector<MaterialPtr> materials;

        ITextureManager * pTextureManager;
        IShaderManager * pShaderManager;

    };
}
