#pragma once

#include <unordered_map>
#include <string>

#include "D3D10Texture.h"

namespace ShiftEngine
{
    struct TextureInfo
    {
        unsigned int width;
        unsigned int height;
    };

    class D3D10TextureManager
    {
    public:
        D3D10TextureManager(ID3D10Device * device, const std::wstring & texturesPath);

        TexturePtr CreateTexture2D(const std::wstring & FileName);
        TexturePtr CreateCubemap(const std::wstring & posX,
                                const std::wstring & negX,
                                const std::wstring & posY,
                                const std::wstring & negY,
                                const std::wstring & posZ,
                                const std::wstring & negZ);
        TexturePtr CreateTextureArray(const std::vector<std::wstring> & names);
        TextureInfo GetTextureInfo(const std::wstring & filename);
        TexturePtr GetErrorTexture();

    private:
        void CreateErrorTexture();

        std::wstring errorName;
        std::wstring texturesPath;
        std::unordered_map<std::wstring, TexturePtr> textures;
        TexturePtr errorTexture;
        ID3D10Device * device;
    };
}
