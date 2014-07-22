#include "cRenderTarget.h"

cRenderTarget::cRenderTarget(ID3D10RenderTargetView * _rt, ID3D10Texture2D * _tex)
	: rt(_rt), tex(_tex) {}

cRenderTarget::~cRenderTarget()
{
	if(rt)
		rt->Release();
	if(tex)
		tex->Release();
}