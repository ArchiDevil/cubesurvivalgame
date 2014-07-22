#pragma once

#if defined (WIN32) || (_WIN32)

    #if defined(__GNUC__) || defined(__clang__)
        #define __in
        #define __inout
        #define __out
        #define __in_opt
        #define __inout_opt
        #define __out_opt
        #define __in_bcount(x)
        #define __out_bcount(x)
        #define __in_bcount_opt(x)
        #define __out_bcount_opt(x)
        #define __in_ecount(x)
        #define __out_ecount(x)
        #define __in_ecount_opt(x)
        #define __out_ecount_opt(x)
        #define __out_ecount_part_opt(x,y)
    #endif //GCC Clang

#include <d3d10.h>

    #if defined(__GNUC__) || defined(__clang__)
        #undef __in
        #undef __inout
        #undef __out
        #undef __in_opt
        #undef __inout_opt
        #undef __out_opt
        #undef __in_bcount(x)
        #undef __out_bcount(x)
        #undef __in_bcount_opt(x)
        #undef __out_bcount_opt(x)
        #undef __in_ecount(x)
        #undef __out_ecount(x)
        #undef __in_ecount_opt(x)
        #undef __out_ecount_opt(x)
        #undef __out_ecount_part_opt(x,y)
    #endif //GCC Clang

#endif //WIN32

#include "cRenderTarget.h"

struct cD3D10
{
	ID3D10Device *				Device;
	IDXGISwapChain *			SwapChain;
	cRenderTarget *				DefaultRT;
	ID3D10Texture2D *			DepthStencilBuffer;
	ID3D10DepthStencilView *	DepthStencilView;

	ID3D10DepthStencilState *	dsStateZOn;
	ID3D10DepthStencilState *	dsStateZOff;

	ID3D10RasterizerState *		rsWireframe;
	ID3D10RasterizerState *		rsNormal;
	ID3D10RasterizerState *		rsNoCulling;

	ID3D10BlendState *			bsAlpha;
	ID3D10BlendState *			bsNormal;
	ID3D10BlendState *			bsAdditive;

	cD3D10()
		: Device(nullptr), SwapChain(nullptr),
		DepthStencilBuffer(nullptr), DepthStencilView(nullptr),
		dsStateZOn(nullptr), dsStateZOff(nullptr),
		rsWireframe(nullptr), rsNormal(nullptr),
		bsAlpha(nullptr), bsNormal(nullptr),
		bsAdditive(nullptr), rsNoCulling(nullptr)
	{
		DefaultRT = new cRenderTarget(nullptr, nullptr);
	}

	~cD3D10()
	{
		if(Device)
			Device->ClearState();
		if(bsNormal)
			bsNormal->Release();
		if(bsAlpha)
			bsAlpha->Release();
		if(bsAdditive)
			bsAdditive->Release();
		if(dsStateZOn)
			dsStateZOn->Release();
		if(dsStateZOff)
			dsStateZOff->Release();
		if(rsNormal)
			rsNormal->Release();
		if(rsWireframe)
			rsWireframe->Release();
		if(DepthStencilView)
			DepthStencilView->Release();
		if(DepthStencilBuffer)
			DepthStencilBuffer->Release();
		if(DefaultRT)
			delete DefaultRT;
//		g_SwapChain->Release();
//		g_Device->Release();
	}

	bool CreateStates()
	{
		D3D10_BLEND_DESC bdDescNormal;
		ZeroMemory(&bdDescNormal, sizeof(D3D10_BLEND_DESC));
		bdDescNormal.BlendEnable[0] = false;
		bdDescNormal.AlphaToCoverageEnable = false;
		bdDescNormal.BlendOp = D3D10_BLEND_OP_MAX;
		bdDescNormal.DestBlend = D3D10_BLEND_DEST_COLOR;
		bdDescNormal.SrcBlend = D3D10_BLEND_SRC_COLOR;
		bdDescNormal.RenderTargetWriteMask[0] = 0x0F;
		if(FAILED(Device->CreateBlendState(&bdDescNormal, &bsNormal)))
			return false;

		D3D10_BLEND_DESC bdDescAlpha;
		ZeroMemory(&bdDescAlpha, sizeof(D3D10_BLEND_DESC));
		bdDescAlpha.AlphaToCoverageEnable = false;
		bdDescAlpha.BlendEnable[0] = true;
		bdDescAlpha.BlendOp = D3D10_BLEND_OP_ADD;
		bdDescAlpha.BlendOpAlpha = D3D10_BLEND_OP_ADD;
		bdDescAlpha.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
		bdDescAlpha.SrcBlend = D3D10_BLEND_SRC_ALPHA;
		bdDescAlpha.DestBlendAlpha = D3D10_BLEND_ONE;
		bdDescAlpha.SrcBlendAlpha = D3D10_BLEND_ONE;
		bdDescAlpha.RenderTargetWriteMask[0] = 0x0F;
		if(FAILED(Device->CreateBlendState(&bdDescAlpha, &bsAlpha)))
			return false;

		D3D10_BLEND_DESC bdDescAdditive;
		ZeroMemory(&bdDescAdditive, sizeof(D3D10_BLEND_DESC));
		bdDescAdditive.AlphaToCoverageEnable = false;
		bdDescAdditive.BlendEnable[0] = true;
		bdDescAdditive.BlendOp = D3D10_BLEND_OP_ADD;
		bdDescAdditive.BlendOpAlpha = D3D10_BLEND_OP_ADD;
		bdDescAdditive.DestBlend = D3D10_BLEND_ONE;
		bdDescAdditive.SrcBlend = D3D10_BLEND_ONE;
		bdDescAdditive.DestBlendAlpha = D3D10_BLEND_ONE;
		bdDescAdditive.SrcBlendAlpha = D3D10_BLEND_ONE;
		bdDescAdditive.RenderTargetWriteMask[0] = 0x0F;
		if(FAILED(Device->CreateBlendState(&bdDescAdditive, &bsAdditive)))
			return false;

		D3D10_DEPTH_STENCIL_DESC dsDescZOn;
		ZeroMemory(&dsDescZOn, sizeof(D3D10_DEPTH_STENCIL_DESC));
		dsDescZOn.DepthEnable = true;
		dsDescZOn.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
		dsDescZOn.DepthFunc = D3D10_COMPARISON_LESS_EQUAL;
		if(FAILED(Device->CreateDepthStencilState(&dsDescZOn, &dsStateZOn)))
			return false;

		D3D10_DEPTH_STENCIL_DESC dsDescZOff;
		ZeroMemory(&dsDescZOff, sizeof(D3D10_DEPTH_STENCIL_DESC));
		dsDescZOff.DepthEnable = false;
		if(FAILED(Device->CreateDepthStencilState(&dsDescZOff, &dsStateZOff)))
			return false;

		D3D10_RASTERIZER_DESC rsDescNormal;
		ZeroMemory(&rsDescNormal, sizeof(D3D10_RASTERIZER_DESC));
		rsDescNormal.FillMode = D3D10_FILL_SOLID;
		rsDescNormal.CullMode = D3D10_CULL_BACK;
		rsDescNormal.FrontCounterClockwise = false;
		rsDescNormal.MultisampleEnable = true;
		if(FAILED(Device->CreateRasterizerState(&rsDescNormal, &rsNormal)))
			return false;

		D3D10_RASTERIZER_DESC rsDescWireframe;
		ZeroMemory(&rsDescWireframe, sizeof(D3D10_RASTERIZER_DESC));
		rsDescWireframe.FillMode = D3D10_FILL_WIREFRAME;
		rsDescWireframe.CullMode = D3D10_CULL_NONE;
		rsDescWireframe.FrontCounterClockwise = false;
		rsDescWireframe.MultisampleEnable = true;
		if(FAILED(Device->CreateRasterizerState(&rsDescWireframe, &rsWireframe)))
			return false;

		D3D10_RASTERIZER_DESC rsDescNoCulling;
		ZeroMemory(&rsDescNoCulling, sizeof(D3D10_RASTERIZER_DESC));
		rsDescNoCulling.FillMode = D3D10_FILL_SOLID;
		rsDescNoCulling.CullMode = D3D10_CULL_NONE;
		rsDescNoCulling.FrontCounterClockwise = false;
		rsDescNoCulling.MultisampleEnable = true;
		if(FAILED(Device->CreateRasterizerState(&rsDescNoCulling, &rsNoCulling)))
			return false;

		return true;
	}

private:
	cD3D10(const cD3D10 & ref);
	cD3D10 & operator=(const cD3D10 & ref);
};
