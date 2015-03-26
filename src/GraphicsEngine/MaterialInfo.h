#pragma once

#include <string>
#include <cstdint>
#include <bitset>
#include <vector>

#include "MiscTypes.h"

namespace ShiftEngine
{
    enum class MaterialLightingModel : uint8_t
    {
        //TODO: NOT SUPPORTED NOW
        None,
        Phong,
        Blinn,
        CookTorrance,
        Minnaert
    };

    //some of these flags are necessary to avoid shader recompiling
    struct materialFlags
    {
        materialFlags();

        bool                    useVertexColors;    //NOT SUPPORTED NOW
        MaterialLightingModel   lightingModel;
        bool                    twoFace;            //NOT SUPPORTED NOW
        bool                    isReflective;       //NOT SUPPORTED NOW
        bool                    isTransparent;
        bool                    isCustom;           //NOT SUPPORTED NOW

        // temporary here
        bool useAnimation;                          //NOT SUPPORTED NOW

        uint32_t GetHash() const;

        bool operator==(const materialFlags & ref) const;
    };

    // additional classes to help with some loading issues
    class MaterialTextureDescription
    {
    public:
        MaterialTextureDescription(TextureType _type = TextureType::Unknown);

        TextureType GetType() const;
        const std::vector<std::string> & GetMaps() const;
        void SetMaps(const std::vector<std::string> & maps);

        bool operator==(const MaterialTextureDescription & ref) const;

    private:
        TextureType type;
        std::vector<std::string> maps;
    };

    class MaterialInfo
    {
        friend class MaterialManager;
    public:
        MaterialInfo();
        MaterialInfo(const materialFlags & _flags);

        const materialFlags * GetFlags() const;
        uint64_t GetHash() const;
        bool operator==(const MaterialInfo & ref) const;
        bool operator<(const MaterialInfo & ref) const;

    public:
        MathLib::Vector4F diffuseColor;
        MathLib::Vector4F specularColor;
        MathLib::Vector4F emissionColor;
        float opacity;
        float shininess;

    private:
        materialFlags flags;

    public:
        //maps should be located at the end of class!

        MaterialTextureDescription diffuseMap;
        MaterialTextureDescription alphaMap;
        //MaterialTextureDescription normalMap;
        //MaterialTextureDescription specularMap;
        //MaterialTextureDescription environmentMap;
    };
}
