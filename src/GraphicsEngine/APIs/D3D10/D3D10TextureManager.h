#pragma once

#include <unordered_map>
#include <string>

#include "../../ITextureManager.h"

#include "D3D10Texture.h"

namespace ShiftEngine
{
    class D3D10TextureManager : public ITextureManager
    {
    public:
        D3D10TextureManager(ID3D10Device * device, const std::wstring & texturesPath);

        ITexturePtr CreateTexture2D(const std::wstring & FileName) override;
        ITexturePtr CreateCubemap(const std::wstring & posX,
                                const std::wstring & negX,
                                const std::wstring & posY,
                                const std::wstring & negY,
                                const std::wstring & posZ,
                                const std::wstring & negZ) override;
        ITexturePtr CreateTextureArray(const std::vector<std::wstring> & names) override;
        TextureInfo GetTextureInfo(const std::wstring & filename) override;
        ITexturePtr GetErrorTexture() override;

    private:
        void CreateErrorTexture();

        std::wstring errorName;
        std::wstring texturesPath;
        std::unordered_map<std::wstring, D3D10TexturePtr> textures;
        D3D10TexturePtr errorTexture;
        ID3D10Device * device;
    };
}
