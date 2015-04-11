#include "D3D10Texture.h"

#include <cassert>

ShiftEngine::D3D10Texture::D3D10Texture(ID3D10Device * pDevice, size_t width, size_t height, TextureType type, ID3D10ShaderResourceView * texture /*= nullptr*/)
    : ITexture(width, height, type)
    , texture(texture)
    , pDevice(pDevice)
{
}

ShiftEngine::D3D10Texture::~D3D10Texture()
{
    if (texture)
        texture->Release();
}

void ShiftEngine::D3D10Texture::Bind(unsigned int index, BindingPoint point)
{
    switch (point)
    {
    case ShiftEngine::BindingPoint::Vertex:
        pDevice->VSSetShaderResources(index, 1, &texture);
        break;
    case ShiftEngine::BindingPoint::Pixel:
        pDevice->PSSetShaderResources(index, 1, &texture);
        break;
    case ShiftEngine::BindingPoint::Geometry:
        pDevice->GSSetShaderResources(index, 1, &texture);
        break;
    default:
        assert(false);
        break;
    }
}
