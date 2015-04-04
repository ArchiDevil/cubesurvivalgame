#include "D3D10RenderTarget.h"

D3D10RenderTarget::D3D10RenderTarget(ID3D10RenderTargetView * _rt, ID3D10Texture2D * _tex)
    : rt(_rt)
    , tex(_tex) 
{
}

D3D10RenderTarget::~D3D10RenderTarget()
{
    if (rt)
        rt->Release();
    if (tex)
        tex->Release();
}
