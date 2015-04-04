#include "ShiftEngine.h"
#include "MaterialManager.h"

#include <regex>
#include <tinyxml2.h>
#include <Utilities/ut.h>

using namespace tinyxml2;

#define PARSING_ERROR(x)                                                            \
    {                                                                               \
        char buffer[1024] = {0};                                                    \
        sprintf_s(buffer, "Error code: %d: %s", result, materialDoc.GetErrorStr1());\
        std::string buf = buffer;                                                   \
        LOG_ERROR(buf);                                                             \
        LOG_ERROR(##x);                                                             \
        LOG_ERROR("Unable to load material ", utils::Narrow(filename));             \
        return nullptr;                                                             \
    }                                                                               \

#define FIND_FLOAT(x)                                                \
    {                                                                \
        std::string str = ""#x;                                      \
        auto iterator = KeyVal.find(str);                            \
        if(iterator != KeyVal.end())                                 \
        {                                                            \
            std::string value = iterator->second->Attribute("value");\
            double val = atof(value.c_str());                        \
            info.##x = (float)val;                                   \
        }                                                            \
    }                                                                \

#define FIND_FLAG(x)                                                 \
    {                                                                \
        std::string str = ""#x;                                      \
        auto iterator = KeyVal.find(str);                            \
        if(iterator != KeyVal.end())                                 \
        {                                                            \
            std::string value = iterator->second->Attribute("value");\
            if(value == "true")                                      \
            {                                                        \
                info.flags.##x = true;                               \
            }                                                        \
            else if(value == "false")                                \
            {                                                        \
                info.flags.##x = false;                              \
            }                                                        \
            else                                                     \
            {                                                        \
                LOG_ERROR("Unable to parse material");               \
                return false;                                        \
            }                                                        \
        }                                                            \
    }                                                                \

ShiftEngine::MaterialManager::MaterialManager(D3D10TextureManager * _manager, IShaderManager * _shaderManager)
    : pTextureManager(_manager)
    , pShaderManager(_shaderManager)
{
}

ShiftEngine::MaterialManager::~MaterialManager()
{
}

ShiftEngine::MaterialPtr ShiftEngine::MaterialManager::LoadMaterial(const std::wstring & filename, const std::wstring & mtlName /*= L"default"*/)
{
    std::map<std::string, XMLElement*> keyVal;	//name/node map

    tinyxml2::XMLDocument materialDoc;
    auto result = materialDoc.LoadFile(utils::Narrow(filename).c_str());

    if (result != XML_NO_ERROR)
    {
        PARSING_ERROR("Unable to open material");
    }

    auto root = materialDoc.RootElement();
    if (!root)
    {
        PARSING_ERROR("Wrong format");
    }

    auto materialNode = root->FirstChildElement("material");
    if (!materialNode)
    {
        PARSING_ERROR("Unable to find any materials");
    }

    for (;;)
    {
        if (!materialNode)
        {
            PARSING_ERROR("Unable to find material");
        }
        //find material name
        auto name = materialNode->Attribute("name");
        if (!name)
        {
            PARSING_ERROR("Unable to parse name of material");
        }

        if (!CheckMaterialName(name))
        {
            PARSING_ERROR("Wrong material name");
        }

        if (mtlName == L"default")
        {
            break;
        }
        else
        {
            if (utils::Narrow(mtlName) == std::string(name))
            {
                break;
            }
            else
            {
                materialNode = materialNode->NextSiblingElement();
            }
        }
    }

    //if we have custom shader, then we can ignore params

    std::string shaderName = "";
    auto customNode = materialNode->FirstChildElement("custom");
    if (customNode)
    {
        auto shaderNode = customNode->FirstChildElement("shader");
        if (shaderNode)
        {
            //save name and api
            shaderName = shaderNode->Attribute("name");
            std::string apiName = shaderNode->Attribute("api");
            auto bindsNode = customNode->FirstChildElement("binds");
            bindsNode;
            //binds just ignored now
            auto uniformsNode = customNode->FirstChildElement("uniforms");
            uniformsNode;
            //uniforms also ingored
        }
        else
        {
            PARSING_ERROR("Wrong param description");
        }
    }

    MaterialPtr pMaterial = nullptr;
    MaterialInfo mtlInfo;

    auto paramsNode = materialNode->FirstChildElement("params");

    if (!paramsNode && !customNode)
    {
        PARSING_ERROR("Wrong material description");
    }
    else if (paramsNode)
    {
        //loading params list
        //without inserted elements now
        auto element = paramsNode->FirstChildElement();
        while (element != nullptr)
        {
            auto attrib = element->Attribute("name");
            if (!attrib)
            {
                PARSING_ERROR("Wrong param description");
            }

            keyVal[attrib] = element;
            element = element->NextSiblingElement();
        }

        if (!FillProperties(mtlInfo, keyVal))
        {
            LOG_ERROR("Unable to fill properties in material manager");
            return nullptr;
        }
    }

    if (!shaderName.empty())
        pMaterial = std::make_shared<Material>(ShiftEngine::GetContextManager()->LoadShader(utils::Widen(shaderName)), mtlInfo);
    else
        pMaterial = std::make_shared<Material>(mtlInfo);

    //loading textures here
    if (pTextureManager)
    {
        LoadTextures(mtlInfo, pMaterial);
    }

    materials.push_back(pMaterial);
    return pMaterial;
}

bool ShiftEngine::MaterialManager::CheckMaterialName(const std::string & name)
{
    std::regex nameRegex("^[A-Za-z][\\w]+");
    if (!std::regex_match(name, nameRegex))
    {
        LOG_ERROR("Wrong material name: ", name, ". Make sure that material name starts with letter and contains only digits, lettes and underlines.");
        return false;
    }

    return true;
}

bool ShiftEngine::MaterialManager::FillProperties(MaterialInfo & info, const std::map<std::string, XMLElement*> & KeyVal)
{
    //loading maps
    if (!LoadMaps(info, KeyVal))
        return false;

    if (!LoadColors(info, KeyVal))
        return false;

    if (!LoadOtherFlags(info, KeyVal))
        return false;

    return true;
}

bool ShiftEngine::MaterialManager::LoadTextures(MaterialInfo & info, MaterialPtr & ptr)
{
    if (info.diffuseMap.GetType() != TextureType::Unknown)
    {
        //TODO: form map here, but now it works only for 2d textures!
        std::wstring wstr = utils::Widen(info.diffuseMap.GetMaps()[0]);
        auto tex = pTextureManager->CreateTexture2D(wstr);
        if (!tex)
            tex = pTextureManager->GetErrorTexture();
        ptr->SetDiffuseTexture(tex);
    }

    if (info.alphaMap.GetType() != TextureType::Unknown)
    {
        //TODO: form map here, but now it works only for 2d textures!
        std::wstring wstr = utils::Widen(info.alphaMap.GetMaps()[0]);
        auto tex = pTextureManager->CreateTexture2D(wstr);
        if (!tex)
            tex = pTextureManager->GetErrorTexture();
        ptr->SetAlphaTexture(tex);
    }

    return true;
}

bool ShiftEngine::MaterialManager::LoadMaps(MaterialInfo & info, const std::map<std::string, XMLElement*> & KeyVal)
{
    //load diffuse map
    auto it = KeyVal.find("diffuseMap");
    if (it != KeyVal.end())
    {
        std::string attr = it->second->Attribute("type");
        if (attr == "")
        {
            LOG_ERROR("Unable to get attribute 'type'");
            info.diffuseMap = MaterialTextureDescription(TextureType::Unknown);
        }
        else
        {
            std::vector<std::string> diffuseMaps;
            auto textureIt = it->second->FirstChildElement();
            if (!textureIt)
            {
                LOG_ERROR("Unable to get textures from texture subitem");
                info.diffuseMap = MaterialTextureDescription(TextureType::Unknown);
            }

            if (attr == "texture2D")
            {
                info.diffuseMap = MaterialTextureDescription(TextureType::Texture2D);
                std::string textureName = textureIt->GetText();
                diffuseMaps.push_back(textureName);
            }

            if (attr == "textureArray")
            {
                std::string textureName;
                info.diffuseMap = MaterialTextureDescription(TextureType::Texture2DArray);
                do
                {
                    textureName = textureIt->GetText();
                    diffuseMaps.push_back(textureName);
                    textureIt = textureIt->NextSiblingElement();
                } while (textureIt != nullptr);
            }

            info.diffuseMap.SetMaps(diffuseMaps);
        }
    }
    else
    {
        info.diffuseMap = MaterialTextureDescription(TextureType::Unknown);
    }

    //load alpha map
    it = KeyVal.find("alphaMap");
    if (it != KeyVal.end())
    {
        std::string attr = it->second->Attribute("type");
        if (attr == "")
        {
            LOG_ERROR("Unable to get attribute 'type'");
            info.alphaMap = MaterialTextureDescription(TextureType::Unknown);
        }
        else
        {
            std::vector<std::string> alphaMaps;
            auto textureIt = it->second->FirstChildElement();
            if (!textureIt)
            {
                LOG_ERROR("Unable to get textures from texture subitem");
                info.alphaMap = MaterialTextureDescription(TextureType::Unknown);
            }

            if (attr == "texture2D")
            {
                info.alphaMap = MaterialTextureDescription(TextureType::Texture2D);
                std::string textureName = textureIt->GetText();
                alphaMaps.push_back(textureName);
            }

            if (attr == "textureArray")
            {
                std::string textureName;
                info.alphaMap = MaterialTextureDescription(TextureType::Texture2DArray);
                do
                {
                    textureName = textureIt->GetText();
                    alphaMaps.push_back(textureName);
                    textureIt = textureIt->NextSiblingElement();
                } while (textureIt != nullptr);
            }

            info.alphaMap.SetMaps(alphaMaps);
        }
    }
    else
    {
        info.alphaMap = MaterialTextureDescription(TextureType::Unknown);
    }

    if (info.alphaMap.GetType() != TextureType::Unknown)
        info.flags.isTransparent = true;

    return true;
}

bool ShiftEngine::MaterialManager::LoadColors(MaterialInfo & info, const std::map<std::string, XMLElement*> & KeyVal)
{
    std::regex checkRegex("rgb\\(\\d+\\.\\d+\\,\\s*\\d+\\.\\d+\\,\\s*\\d+\\.\\d+\\)");
    std::regex numRegex("\\d+\\.\\d+");
    std::smatch match;
    std::string nums[3];

#define GET_COLOR(x)                                                                            \
    {                                                                                           \
        auto iterator = KeyVal.find(""#x);                                                      \
        if(iterator != KeyVal.end())                                                            \
        {                                                                                       \
            std::string value = iterator->second->Attribute("value");                           \
            if(!std::regex_match(value, checkRegex))                                            \
            {                                                                                   \
                LOG_ERROR(value, " is not correct value for color");                            \
                return false;                                                                   \
            }                                                                                   \
                                                                                                \
            for(int i = 0; i < 3; i++)                                                          \
            {                                                                                   \
                std::regex_search(value, match, numRegex);                                      \
                auto string = match.str();                                                      \
                nums[i] = string;                                                               \
                value = match.suffix();                                                         \
            }                                                                                   \
                                                                                                \
            info.##x = MathLib::Vector4F(stof(nums[0]), stof(nums[1]), stof(nums[2]), 1.0f);    \
        }                                                                                       \
    }                                                                                           \

    GET_COLOR(diffuseColor);
    GET_COLOR(specularColor);
    GET_COLOR(emissionColor);

    return true;
}

bool ShiftEngine::MaterialManager::LoadOtherFlags(MaterialInfo & info, const std::map<std::string, XMLElement*> & KeyVal)
{
    //loading bools first
    FIND_FLAG(twoFace);
    FIND_FLAG(isReflective);
    FIND_FLAG(isTransparent);

    //loading floats here
    FIND_FLOAT(opacity);
    if (info.opacity < 1.0f) info.flags.isTransparent = true;
    FIND_FLOAT(shininess);

    return true;
}
