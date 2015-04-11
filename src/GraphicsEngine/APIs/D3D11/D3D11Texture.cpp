#include "D3D11Texture.h"

#include <cassert>

ShiftEngine::D3D11Texture::D3D11Texture(ID3D11DeviceContext * pDeviceContext, size_t width, size_t height, TextureType type, ID3D11ShaderResourceView * texture /*= nullptr*/)
    : ITexture(width, height, type)
    , texture(texture)
    , pDeviceContext(pDeviceContext)
{
}

ShiftEngine::D3D11Texture::~D3D11Texture()
{
    if (texture)
        texture->Release();
}

void ShiftEngine::D3D11Texture::Bind(unsigned int index, BindingPoint point)
{
    switch (point)
    {
    case ShiftEngine::BindingPoint::Vertex:
        pDeviceContext->VSSetShaderResources(index, 1, &texture);
        break;
    case ShiftEngine::BindingPoint::Pixel:
        pDeviceContext->PSSetShaderResources(index, 1, &texture);
        break;
    case ShiftEngine::BindingPoint::Geometry:
        pDeviceContext->GSSetShaderResources(index, 1, &texture);
        break;
    case ShiftEngine::BindingPoint::Hull:
        pDeviceContext->HSSetShaderResources(index, 1, &texture);
        break;
    case ShiftEngine::BindingPoint::Domain:
        pDeviceContext->DSSetShaderResources(index, 1, &texture);
        break;
    case ShiftEngine::BindingPoint::Compute:
        pDeviceContext->CSSetShaderResources(index, 1, &texture);
        break;
    default:
        assert(false);
        break;
    }
}
