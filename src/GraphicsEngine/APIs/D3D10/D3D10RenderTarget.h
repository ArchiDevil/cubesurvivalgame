#pragma once

#include <D3D10.h>
#include <memory>

struct D3D10RenderTarget
{
    D3D10RenderTarget(ID3D10RenderTargetView * _rt, ID3D10Texture2D * _tex);
    ~D3D10RenderTarget();

    ID3D10RenderTargetView * rt;
    ID3D10Texture2D * tex;
};

typedef std::shared_ptr<D3D10RenderTarget> D3D10RenderTargetPtr;
