#include "D3D10TextureManager.h"

#include <Utilities/ut.h>
#include <Utilities/logger.hpp>

#include <D3DX10.h>
#include <cassert>

ShiftEngine::D3D10TextureManager::D3D10TextureManager(ID3D10Device * device, const std::wstring & texturesPath)
    : texturesPath(texturesPath)
    , device(device)
{
    CreateErrorTexture();
}

ShiftEngine::ITexturePtr ShiftEngine::D3D10TextureManager::CreateTexture2D(const std::wstring & FileName)
{
    std::wstring innerName = texturesPath + FileName;
    auto iter = textures.find(innerName);

    if (iter != textures.end() && iter->second != nullptr)
    {
        return D3D10TexturePtr(iter->second);
    }
    else
    {
        ID3D10ShaderResourceView * tempT;
        D3DX10_IMAGE_INFO info;
        if (FAILED(D3DX10GetImageInfoFromFile(innerName.c_str(), NULL, &info, NULL)) ||
            FAILED(D3DX10CreateShaderResourceViewFromFile(device, innerName.c_str(), NULL, NULL, &tempT, NULL)))
        {
            return nullptr;
        }
        else
        {
            D3D10TexturePtr buffer = std::make_shared<D3D10Texture>(device, info.Width, info.Height, TextureType::Texture2D, tempT);
            textures[innerName] = buffer;
            return buffer;
        }
    }
}

ShiftEngine::ITexturePtr ShiftEngine::D3D10TextureManager::CreateCubemap(const std::wstring & posX,
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
    ID3D10ShaderResourceView * SRW;
    const size_t elems = 6;

    HRESULT hr = S_OK;

    std::vector<D3DX10_IMAGE_INFO> infos;
    for (UINT i = 0; i < elems; ++i)
    {
        D3DX10_IMAGE_INFO ii;
        if (FAILED(D3DX10GetImageInfoFromFile((texturesPath + items[i]).c_str(), nullptr, &ii, &hr)))
            LOG_ERROR("Unable to get image info for: ", utils::Narrow(items[i]));

        infos.push_back(ii);
    }

    if (infos.size() > 1)
    {
        size_t prevWidth = infos[0].Width;
        size_t prevHeight = infos[0].Height;
        for (UINT i = 1; i < infos.size(); ++i)
        {
            if (infos[i].Width != prevWidth || infos[i].Height != prevHeight)
            {
                LOG_FATAL_ERROR("Unable to create cubemap from textures without the same size");
                return nullptr;
            }
        }
    }

    ID3D10Texture2D** srcTex = new ID3D10Texture2D *[elems];

    for (UINT i = 0; i < elems; ++i)
    {
        D3DX10_IMAGE_LOAD_INFO loadInfo;

        loadInfo.Width = D3DX10_FROM_FILE;
        loadInfo.Height = D3DX10_FROM_FILE;
        loadInfo.Depth = 1;
        loadInfo.FirstMipLevel = 0;
        loadInfo.MipLevels = 0; //default
        loadInfo.Usage = D3D10_USAGE_STAGING;
        loadInfo.BindFlags = 0;
        loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
        loadInfo.MiscFlags = 0;
        loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        loadInfo.Filter = D3DX10_FILTER_LINEAR;
        loadInfo.MipFilter = D3DX10_FILTER_LINEAR;
        loadInfo.pSrcInfo = 0;

        if (FAILED(D3DX10CreateTextureFromFile(device,
            (texturesPath + items[i]).c_str(),
            &loadInfo, 0,
            (ID3D10Resource**)&srcTex[i], 0)))
        {
            LOG_ERROR("Unable to load texture in cubemap: ", utils::Narrow(items[i]));

            D3D10_TEXTURE2D_DESC desc;
            desc.ArraySize = 1;
            desc.BindFlags = 0;
            desc.CPUAccessFlags = NULL;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.Height = infos[0].Height;
            desc.Width = infos[0].Width;
            desc.MipLevels = 0;
            desc.MiscFlags = NULL;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Usage = D3D10_USAGE_STAGING;
            //HACK: crappy code!
            std::vector<uint32_t> textureData(infos[0].Height * infos[0].Width);
            for (size_t j = 0; j < infos[0].Height * infos[0].Width; ++j)
            {
                textureData[j] = 0xffff00ff;
            }
            D3D10_SUBRESOURCE_DATA data;
            data.pSysMem = textureData.data();
            device->CreateTexture2D(&desc, &data, &srcTex[i]);
        }
    }


    D3D10_TEXTURE2D_DESC texElementDesc;
    srcTex[0]->GetDesc(&texElementDesc);

    D3D10_TEXTURE2D_DESC texArrayDesc;
    texArrayDesc.Width = texElementDesc.Width;
    texArrayDesc.Height = texElementDesc.Height;
    texArrayDesc.MipLevels = texElementDesc.MipLevels;
    texArrayDesc.ArraySize = elems;
    texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texArrayDesc.SampleDesc.Count = 1;
    texArrayDesc.SampleDesc.Quality = 0;
    texArrayDesc.Usage = D3D10_USAGE_DEFAULT;
    texArrayDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    texArrayDesc.CPUAccessFlags = 0;
    texArrayDesc.MiscFlags = 0;

    ID3D10Texture2D* texArray = nullptr;
    if (FAILED(device->CreateTexture2D(&texArrayDesc, 0, &texArray)))
    {
        LOG_ERROR("Unable to create Cubemap!");
        return nullptr;
    }

    // for each texture element...
    for (UINT i = 0; i < elems; ++i)
    {
        // for each mipmap level...
        for (UINT j = 0; j < texElementDesc.MipLevels; ++j)
        {
            D3D10_MAPPED_TEXTURE2D mappedTex2D;
            srcTex[i]->Map(j, D3D10_MAP_READ, 0, &mappedTex2D);

            device->UpdateSubresource(texArray, D3D10CalcSubresource(j, i, texElementDesc.MipLevels),
                                      0, mappedTex2D.pData, mappedTex2D.RowPitch, 0);

            srcTex[i]->Unmap(j);
        }
    }

    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = texArrayDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
    viewDesc.Texture2DArray.MostDetailedMip = 0;
    viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
    viewDesc.Texture2DArray.FirstArraySlice = 0;
    viewDesc.Texture2DArray.ArraySize = elems;

    if (FAILED(device->CreateShaderResourceView(texArray, &viewDesc, &SRW)))
        LOG_FATAL_ERROR("Unable to create ShaderResourceView");

    auto texturePtr = std::make_shared<D3D10Texture>(device, texElementDesc.Width, texElementDesc.Height, TextureType::TextureCubemap, SRW);
    textures[superString] = texturePtr;
    return texturePtr;
}

ShiftEngine::ITexturePtr ShiftEngine::D3D10TextureManager::CreateTextureArray(const std::vector<std::wstring> & names)
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

    ID3D10ShaderResourceView * SRW;

    const size_t elems = names.size();

    HRESULT hr = S_OK;

    std::vector<D3DX10_IMAGE_INFO> infos;
    //check here info
    for (UINT i = 0; i < elems; ++i)
    {
        D3DX10_IMAGE_INFO ii;
        if (FAILED(D3DX10GetImageInfoFromFile((texturesPath + names[i]).c_str(), nullptr, &ii, &hr)))
        {
            LOG_ERROR("Unable to get image info for: ", utils::Narrow(names[i]));
        }
        infos.push_back(ii);
    }

    if (infos.size() > 1)
    {
        size_t prevWidth = infos[0].Width;
        size_t prevHeight = infos[0].Height;
        for (UINT i = 1; i < infos.size(); ++i)
        {
            if (infos[i].Width != prevWidth || infos[i].Height != prevHeight)
            {
                LOG_FATAL_ERROR("Unable to create texture array from textures without the same size");
            }
        }
    }

    ID3D10Texture2D** srcTex = new ID3D10Texture2D *[elems];

    for (UINT i = 0; i < elems; ++i)
    {
        D3DX10_IMAGE_LOAD_INFO loadInfo;

        loadInfo.Width = D3DX10_FROM_FILE;
        loadInfo.Height = D3DX10_FROM_FILE;
        loadInfo.Depth = 1;
        loadInfo.FirstMipLevel = 0;
        loadInfo.MipLevels = 0; //default
        loadInfo.Usage = D3D10_USAGE_STAGING;
        loadInfo.BindFlags = 0;
        loadInfo.CpuAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
        loadInfo.MiscFlags = 0;
        loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        loadInfo.Filter = D3DX10_FILTER_LINEAR;
        loadInfo.MipFilter = D3DX10_FILTER_LINEAR;
        loadInfo.pSrcInfo = 0;

        if (FAILED(D3DX10CreateTextureFromFile(device,
            (texturesPath + names[i]).c_str(),
            &loadInfo, 0,
            (ID3D10Resource**)&srcTex[i], 0)))
        {
            LOG_ERROR("Unable to load texture in texture array: ", utils::Narrow(names[i]));

            D3D10_TEXTURE2D_DESC desc;
            desc.ArraySize = 1;
            desc.BindFlags = 0;
            desc.CPUAccessFlags = NULL;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.Height = infos[0].Height;
            desc.Width = infos[0].Width;
            desc.MipLevels = 0;
            desc.MiscFlags = NULL;
            desc.SampleDesc.Count = 1;
            desc.SampleDesc.Quality = 0;
            desc.Usage = D3D10_USAGE_STAGING;
            //HACK: crappy code!
            std::vector<uint32_t> textureData(infos[0].Height * infos[0].Width);
            for (size_t j = 0; j < infos[0].Height * infos[0].Width; ++j)
            {
                textureData[j] = 0xffff00ff;
            }
            D3D10_SUBRESOURCE_DATA data;
            data.pSysMem = textureData.data();
            device->CreateTexture2D(&desc, &data, &srcTex[i]);
        }
    }


    D3D10_TEXTURE2D_DESC texElementDesc;
    srcTex[0]->GetDesc(&texElementDesc);

    D3D10_TEXTURE2D_DESC texArrayDesc;
    texArrayDesc.Width = texElementDesc.Width;
    texArrayDesc.Height = texElementDesc.Height;
    texArrayDesc.MipLevels = texElementDesc.MipLevels;
    texArrayDesc.ArraySize = elems;
    texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texArrayDesc.SampleDesc.Count = 1;
    texArrayDesc.SampleDesc.Quality = 0;
    texArrayDesc.Usage = D3D10_USAGE_DEFAULT;
    texArrayDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    texArrayDesc.CPUAccessFlags = 0;
    texArrayDesc.MiscFlags = 0;

    ID3D10Texture2D* texArray = nullptr;
    if (FAILED(device->CreateTexture2D(&texArrayDesc, 0, &texArray)))
        LOG_ERROR("Unable to create TextureArray!");

    // for each texture element...
    for (UINT i = 0; i < elems; ++i)
    {
        // for each mipmap level...
        for (UINT j = 0; j < texElementDesc.MipLevels; ++j)
        {
            D3D10_MAPPED_TEXTURE2D mappedTex2D;
            srcTex[i]->Map(j, D3D10_MAP_READ, 0, &mappedTex2D);

            device->UpdateSubresource(texArray, D3D10CalcSubresource(j, i, texElementDesc.MipLevels),
                                      0, mappedTex2D.pData, mappedTex2D.RowPitch, 0);

            srcTex[i]->Unmap(j);
        }
    }

    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = texArrayDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
    viewDesc.Texture2DArray.MostDetailedMip = 0;
    viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
    viewDesc.Texture2DArray.FirstArraySlice = 0;
    viewDesc.Texture2DArray.ArraySize = elems;

    if (FAILED(device->CreateShaderResourceView(texArray, &viewDesc, &SRW)))
        LOG_FATAL_ERROR("Unable to create ShaderResourceView");

    auto texturePtr = std::make_shared<D3D10Texture>(device, texElementDesc.Width, texElementDesc.Height, TextureType::Texture2DArray, SRW);
    textures[superString] = texturePtr;
    return texturePtr;
}

ShiftEngine::TextureInfo ShiftEngine::D3D10TextureManager::GetTextureInfo(const std::wstring & filename)
{
    D3DX10_IMAGE_INFO info;

    if (SUCCEEDED(D3DX10GetImageInfoFromFile(filename.c_str(), NULL, &info, NULL)))
    {
        TextureInfo out;
        out.width = info.Width;
        out.height = info.Height;
        return out;
    }
    else
    {
        LOG_ERROR("Unable to get info for texture: ", utils::Narrow(filename));
        return TextureInfo();
    }
}

void ShiftEngine::D3D10TextureManager::CreateErrorTexture()
{
    //trying to create texture procedurally
    const unsigned int width = 256;
    const unsigned int height = 256;
    ID3D10Texture2D * texture = nullptr;
    D3D10_TEXTURE2D_DESC description;
    description.ArraySize = 1;
    description.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    description.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    description.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    description.Height = height;
    description.Width = width;
    description.MipLevels = 1;
    description.MiscFlags = 0;
    description.SampleDesc.Count = 1;
    description.SampleDesc.Quality = 0;
    description.Usage = D3D10_USAGE_DYNAMIC;

    if (FAILED(device->CreateTexture2D(&description, nullptr, &texture)))
        LOG_FATAL_ERROR("Unable to create error texture");

    D3D10_MAPPED_TEXTURE2D mappedTexture;
    if (FAILED(texture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, NULL, &mappedTexture)))
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

            assert(rowStart + col * 4 + 3 < 256 * 256 * 4);
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
    texture->Unmap(D3D10CalcSubresource(0, 0, 1));
    // Create the shader-resource view
    D3D10_SHADER_RESOURCE_VIEW_DESC srDesc;
    srDesc.Format = description.Format;
    srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    srDesc.Texture2D.MostDetailedMip = 0;
    srDesc.Texture2D.MipLevels = 1;

    ID3D10ShaderResourceView *pShaderResView = NULL;
    if (FAILED(device->CreateShaderResourceView(texture, &srDesc, &pShaderResView)))
        LOG_FATAL_ERROR("Unable to create error texture");

    texture->Release();

    errorTexture = std::make_shared<D3D10Texture>(device, width, height, TextureType::Texture2D, pShaderResView);
}

ShiftEngine::ITexturePtr ShiftEngine::D3D10TextureManager::GetErrorTexture()
{
    return errorTexture;
}