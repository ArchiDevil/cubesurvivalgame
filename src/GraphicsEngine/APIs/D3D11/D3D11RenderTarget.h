#pragma once

#include <D3D11.h>
#include <memory>

struct D3D11RenderTarget
{
    D3D11RenderTarget(ID3D11RenderTargetView * _rt, ID3D11Texture2D * _tex);
    ~D3D11RenderTarget();

    ID3D11RenderTargetView * rt;
    ID3D11Texture2D * tex;
};

typedef std::shared_ptr<D3D11RenderTarget> D3D11RenderTargetPtr;
