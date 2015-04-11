#pragma once

#include <unordered_map>
#include <string>

#include "../../ITextureManager.h"

#include "D3D11Texture.h"

namespace ShiftEngine
{
    class D3D11TextureManager : public ITextureManager
    {
    public:
        D3D11TextureManager(ID3D11Device * device, ID3D11DeviceContext * pDeviceContext, const std::wstring & texturesPath);

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

        std::wstring errorName = L"";
        std::wstring texturesPath = L"";
        std::unordered_map<std::wstring, D3D11TexturePtr> textures;
        D3D11TexturePtr errorTexture = nullptr;

        ID3D11Device * pDevice = nullptr;
        ID3D11DeviceContext * pDeviceContext = nullptr;
    };
}
