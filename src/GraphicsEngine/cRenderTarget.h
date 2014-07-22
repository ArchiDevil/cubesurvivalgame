#pragma once

#include <D3D10.h>
#include <memory>

struct cRenderTarget
{
	cRenderTarget(ID3D10RenderTargetView * _rt, ID3D10Texture2D * _tex);
	~cRenderTarget();

	ID3D10RenderTargetView * rt;
	ID3D10Texture2D * tex;
};

typedef std::shared_ptr<cRenderTarget> RenderTargetPtr;