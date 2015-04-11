#include "D3D11RenderTarget.h"

D3D11RenderTarget::D3D11RenderTarget(ID3D11RenderTargetView * _rt, ID3D11Texture2D * _tex)
    : rt(_rt)
    , tex(_tex) 
{
}

D3D11RenderTarget::~D3D11RenderTarget()
{
    if (rt)
        rt->Release();
    if (tex)
        tex->Release();
}
