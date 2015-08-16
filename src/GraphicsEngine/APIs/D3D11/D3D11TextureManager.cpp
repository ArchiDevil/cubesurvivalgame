#include "D3D11TextureManager.h"

#include <Utilities/ut.h>
#include <Utilities/logger.hpp>

#include <DirectXTex.h>

ShiftEngine::D3D11TextureManager::D3D11TextureManager(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const std::wstring & texturesPath)
    : texturesPath(texturesPath)
    , pDevice(pDevice)
    , pDeviceContext(pDeviceContext)
{
    CreateErrorTexture();
}

ShiftEngine::ITexturePtr ShiftEngine::D3D11TextureManager::CreateTexture2D(const std::wstring & FileName)
{
    std::wstring innerName = texturesPath + FileName;
    auto iter = textures.find(innerName);

    if (iter != textures.end() && iter->second)
    {
        return D3D11TexturePtr(iter->second);
    }
    else
    {
        ID3D11ShaderResourceView * shaderResource = nullptr;
        ID3D11Texture2D * textureResource = nullptr;
        DirectX::TexMetadata metadata;
        DirectX::ScratchImage image;
        if (FAILED(DirectX::LoadFromWICFile(innerName.c_str(), 0, &metadata, image)))
        {
            LOG_ERROR("Unable to load file: ", utils::Narrow(FileName));
            return GetErrorTexture();
        }

        if (FAILED(DirectX::CreateTexture(pDevice, image.GetImage(0, 0, 0), 1, metadata, (ID3D11Resource**)&textureResource)))
        {
            LOG_ERROR("Unable to create texture from file: ", utils::Narrow(FileName));
            return GetErrorTexture();
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
        viewDesc.Format = metadata.format;
        viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MostDetailedMip = 0;
        viewDesc.Texture2D.MipLevels = metadata.mipLevels;

        if (FAILED(pDevice->CreateShaderResourceView(textureResource, &viewDesc, &shaderResource)))
        {
            LOG_ERROR("Unable to create ShaderResourceView");
            return GetErrorTexture();
        }

        D3D11TexturePtr buffer = std::make_shared<D3D11Texture>(pDeviceContext, metadata.width, metadata.height, TextureType::Texture2D, shaderResource);
        textures[innerName] = buffer;
        return buffer;
    }
}

ShiftEngine::ITexturePtr ShiftEngine::D3D11TextureManager::CreateCubemap(const std::wstring & posX,
    const std::wstring & negX,
    const std::wstring & posY,
    const std::wstring & negY,
    const std::wstring & posZ,
    const std::wstring & negZ)
{
    std::wstring superString = negX + posX + negY + posY + negZ + posZ;

    if (negX.empty() || posX.empty() ||
        negY.empty() || posY.empty() ||
        negZ.empty() || posZ.empty())
    {
        LOG_ERROR("Unable to load cubemap with empty textures");
        return nullptr;
    }

    std::vector<std::wstring> items = { posX, negX, posY, negY, posZ, negZ };
    ID3D11ShaderResourceView * SRW;
    const size_t elems = 6;

    std::vector<DirectX::TexMetadata> metadatas;
    for (size_t i = 0; i < elems; ++i)
    {
        DirectX::TexMetadata ii;
        if (FAILED(DirectX::GetMetadataFromWICFile((texturesPath + items[i]).c_str(), 0, ii)))
            LOG_ERROR("Unable to get image info for: ", utils::Narrow(items[i]));

        metadatas.push_back(ii);
    }

    if (metadatas.size() > 1)
    {
        size_t prevWidth = metadatas[0].width;
        size_t prevHeight = metadatas[0].height;
        for (size_t i = 1; i < metadatas.size(); ++i)
        {
            if (metadatas[i].width != prevWidth || metadatas[i].height != prevHeight)
            {
                LOG_FATAL_ERROR("Unable to create cubemap from textures without the same size");
                return nullptr;
            }
        }
    }

    ID3D11Texture2D** srcTex = new ID3D11Texture2D *[elems];

    for (size_t i = 0; i < elems; ++i)
    {
        bool error = false;
        DirectX::ScratchImage image;
        if (FAILED(DirectX::LoadFromWICFile((texturesPath + items[i]).c_str(), 0, &metadatas[i], image)))
            error = true;

        if (FAILED(DirectX::CreateTexture(pDevice, image.GetImage(0, 0, 0), 1, metadatas[i], (ID3D11Resource**)&srcTex[i])))
            error = true;

        if (error)
        {
            LOG_ERROR("Unable to load texture in cubemap: ", utils::Narrow(items[i]));

            D3D11_TEXTURE2D_DESC desc;
            desc.ArraySize = 1;
            desc.BindFlags = 0;
            desc.CPUAccessFlags = NULL;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.Height = metadatas[0].height;
            desc.Width = metadatas[0].width;
            desc.MipLevels = 0;
            desc.MiscFlags = NULL;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Usage = D3D11_USAGE_STAGING;
            //HACK: crappy code!
            std::vector<uint32_t> textureData(metadatas[0].height * metadatas[0].width);
            for (size_t j = 0; j < metadatas[0].height * metadatas[0].width; ++j)
            {
                textureData[j] = 0xffff00ff;
            }
            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = textureData.data();
            pDevice->CreateTexture2D(&desc, &data, &srcTex[i]);
        }
    }

    D3D11_TEXTURE2D_DESC texElementDesc;
    srcTex[0]->GetDesc(&texElementDesc);

    D3D11_TEXTURE2D_DESC texArrayDesc;
    texArrayDesc.Width = texElementDesc.Width;
    texArrayDesc.Height = texElementDesc.Height;
    texArrayDesc.MipLevels = texElementDesc.MipLevels;
    texArrayDesc.ArraySize = elems;
    texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texArrayDesc.SampleDesc.Count = 1;
    texArrayDesc.SampleDesc.Quality = 0;
    texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
    texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texArrayDesc.CPUAccessFlags = 0;
    texArrayDesc.MiscFlags = 0;

    ID3D11Texture2D* texArray = nullptr;
    if (FAILED(pDevice->CreateTexture2D(&texArrayDesc, 0, &texArray)))
    {
        LOG_ERROR("Unable to create Cubemap!");
        return nullptr;
    }

    // for each texture element...
    for (size_t i = 0; i < elems; ++i)
    {
        // for each mipmap level...
        for (UINT j = 0; j < texElementDesc.MipLevels; ++j)
        {
            D3D11_MAPPED_SUBRESOURCE mappedTex2D;
            pDeviceContext->Map(srcTex[i], 0, D3D11_MAP_WRITE, 0, &mappedTex2D);
            pDeviceContext->UpdateSubresource(texArray, D3D11CalcSubresource(j, i, texElementDesc.MipLevels),
                0, mappedTex2D.pData, mappedTex2D.RowPitch, 0);
            pDeviceContext->Unmap(srcTex[i], 0);
        }
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = texArrayDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    viewDesc.Texture2DArray.MostDetailedMip = 0;
    viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
    viewDesc.Texture2DArray.FirstArraySlice = 0;
    viewDesc.Texture2DArray.ArraySize = elems;

    if (FAILED(pDevice->CreateShaderResourceView(texArray, &viewDesc, &SRW)))
        LOG_FATAL_ERROR("Unable to create ShaderResourceView");

    auto texturePtr = std::make_shared<D3D11Texture>(pDeviceContext, texElementDesc.Width, texElementDesc.Height, TextureType::TextureCubemap, SRW);
    textures[superString] = texturePtr;
    return texturePtr;
}

ShiftEngine::ITexturePtr ShiftEngine::D3D11TextureManager::CreateTextureArray(const std::vector<std::wstring> & names)
{
    std::wstring superString = L"";
    for (auto & elem : names)
    {
        superString += elem;
    }

    if (names.size() < 2)
    {
        LOG_ERROR("Unable to load texture array with texture count < 2");
        return nullptr;
    }

    ID3D11ShaderResourceView * SRW;

    const size_t elems = names.size();

    HRESULT hr = S_OK;

    std::vector<DirectX::TexMetadata> metadatas;
    //check here info
    for (size_t i = 0; i < elems; ++i)
    {
        DirectX::TexMetadata ii;
        if (FAILED(DirectX::GetMetadataFromWICFile((texturesPath + names[i]).c_str(), 0, ii)))
            LOG_ERROR("Unable to get image info for: ", utils::Narrow(names[i]));

        metadatas.push_back(ii);
    }

    if (metadatas.size() > 1)
    {
        size_t prevWidth = metadatas[0].width;
        size_t prevHeight = metadatas[0].height;
        for (size_t i = 1; i < metadatas.size(); ++i)
        {
            if (metadatas[i].width != prevWidth || metadatas[i].height != prevHeight)
            {
                LOG_FATAL_ERROR("Unable to create texture array from textures without the same size");
            }
        }
    }

    ID3D11Texture2D** srcTex = new ID3D11Texture2D *[elems];

    for (size_t i = 0; i < elems; ++i)
    {
        bool error = false;
        DirectX::ScratchImage image;
        if (FAILED(DirectX::LoadFromWICFile((texturesPath + names[i]).c_str(), 0, &metadatas[i], image)))
            error = true;

        if (FAILED(DirectX::CreateTexture(pDevice, image.GetImage(0, 0, 0), 1, metadatas[i], (ID3D11Resource**)&srcTex[i])))
            error = true;

        if (error)
        {
            LOG_ERROR("Unable to load texture in texture array: ", utils::Narrow(names[i]));

            D3D11_TEXTURE2D_DESC desc;
            desc.ArraySize = 1;
            desc.BindFlags = 0;
            desc.CPUAccessFlags = NULL;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.Height = metadatas[0].height;
            desc.Width = metadatas[0].width;
            desc.MipLevels = 0;
            desc.MiscFlags = NULL;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Usage = D3D11_USAGE_STAGING;
            //HACK: crappy code!
            std::vector<uint32_t> textureData(metadatas[0].height * metadatas[0].width);
            for (size_t j = 0; j < metadatas[0].height * metadatas[0].width; ++j)
            {
                textureData[j] = 0xffff00ff;
            }
            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = textureData.data();
            pDevice->CreateTexture2D(&desc, &data, &srcTex[i]);
        }
    }


    D3D11_TEXTURE2D_DESC texElementDesc;
    srcTex[0]->GetDesc(&texElementDesc);

    D3D11_TEXTURE2D_DESC texArrayDesc;
    texArrayDesc.Width = texElementDesc.Width;
    texArrayDesc.Height = texElementDesc.Height;
    texArrayDesc.MipLevels = texElementDesc.MipLevels;
    texArrayDesc.ArraySize = elems;
    texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texArrayDesc.SampleDesc.Count = 1;
    texArrayDesc.SampleDesc.Quality = 0;
    texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
    texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texArrayDesc.CPUAccessFlags = 0;
    texArrayDesc.MiscFlags = 0;

    ID3D11Texture2D* texArray = nullptr;
    if (FAILED(pDevice->CreateTexture2D(&texArrayDesc, 0, &texArray)))
        LOG_ERROR("Unable to create TextureArray!");

    // for each texture element...
    for (size_t i = 0; i < elems; ++i)
    {
        // for each mipmap level...
        for (UINT j = 0; j < texElementDesc.MipLevels; ++j)
        {
            D3D11_MAPPED_SUBRESOURCE mappedTex2D;
            pDeviceContext->Map(srcTex[i], 0, D3D11_MAP_WRITE, 0, &mappedTex2D);
            pDeviceContext->UpdateSubresource(texArray, D3D11CalcSubresource(j, i, texElementDesc.MipLevels),
                0, mappedTex2D.pData, mappedTex2D.RowPitch, 0);
            pDeviceContext->Unmap(srcTex[i], 0);
        }
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = texArrayDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    viewDesc.Texture2DArray.MostDetailedMip = 0;
    viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
    viewDesc.Texture2DArray.FirstArraySlice = 0;
    viewDesc.Texture2DArray.ArraySize = elems;

    if (FAILED(pDevice->CreateShaderResourceView(texArray, &viewDesc, &SRW)))
        LOG_FATAL_ERROR("Unable to create ShaderResourceView");

    auto texturePtr = std::make_shared<D3D11Texture>(pDeviceContext, texElementDesc.Width, texElementDesc.Height, TextureType::Texture2DArray, SRW);
    textures[superString] = texturePtr;
    return texturePtr;
}

ShiftEngine::TextureInfo ShiftEngine::D3D11TextureManager::GetTextureInfo(const std::wstring & filename)
{
    DirectX::TexMetadata info;

    if (SUCCEEDED(DirectX::GetMetadataFromWICFile(filename.c_str(), 0, info)))
    {
        TextureInfo out;
        out.width = info.width;
        out.height = info.height;
        return out;
    }
    else
    {
        LOG_ERROR("Unable to get info for texture: ", utils::Narrow(filename));
        return TextureInfo();
    }
}

void ShiftEngine::D3D11TextureManager::CreateErrorTexture()
{
    //trying to create texture procedurally
    const unsigned int width = 256;
    const unsigned int height = 256;
    ID3D11Texture2D * texture = nullptr;
    D3D11_TEXTURE2D_DESC description;
    description.ArraySize = 1;
    description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    description.Height = height;
    description.Width = width;
    description.MipLevels = 1;
    description.MiscFlags = 0;
    description.SampleDesc.Count = 1;
    description.SampleDesc.Quality = 0;
    description.Usage = D3D11_USAGE_DYNAMIC;

    if (FAILED(pDevice->CreateTexture2D(&description, nullptr, &texture)))
        LOG_FATAL_ERROR("Unable to create error texture");

    D3D11_MAPPED_SUBRESOURCE mappedTexture;
    if (FAILED(pDeviceContext->Map(texture, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, NULL, &mappedTexture)))
    {
        texture->Release();
        LOG_FATAL_ERROR("Unable to create error texture");
    }

    uint8_t * pTexels = (uint8_t*)mappedTexture.pData;
    for (UINT row = 0; row < height; row++)
    {
        UINT rowStart = row * mappedTexture.RowPitch;
        for (UINT col = 0; col < width; col++)
        {
            unsigned int xBlock = col * 8 / width;
            unsigned int yBlock = row * 8 / height;
            UINT colStart = col * 4;

            if (xBlock % 2 == yBlock % 2)
            {
                pTexels[rowStart + colStart + 0] = 255; // Red
                pTexels[rowStart + colStart + 1] = 0; // Green
                pTexels[rowStart + colStart + 2] = 0; // Blue
                pTexels[rowStart + colStart + 3] = 255; // Alpha
            }
            else
            {
                pTexels[rowStart + colStart + 0] = 0; // Red
                pTexels[rowStart + colStart + 1] = 255; // Green
                pTexels[rowStart + colStart + 2] = 0; // Blue
                pTexels[rowStart + colStart + 3] = 255; // Alpha
            }
        }
    }
    pDeviceContext->Unmap(texture, D3D11CalcSubresource(0, 0, 1));
    // Create the shader-resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
    srDesc.Format = description.Format;
    srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srDesc.Texture2D.MostDetailedMip = 0;
    srDesc.Texture2D.MipLevels = 1;

    ID3D11ShaderResourceView *pShaderResView = NULL;
    if (FAILED(pDevice->CreateShaderResourceView(texture, &srDesc, &pShaderResView)))
        LOG_FATAL_ERROR("Unable to create error texture");

    texture->Release();

    errorTexture = std::make_shared<D3D11Texture>(pDeviceContext, width, height, TextureType::Texture2D, pShaderResView);
}

ShiftEngine::ITexturePtr ShiftEngine::D3D11TextureManager::GetErrorTexture()
{
    return errorTexture;
}
