#pragma once

#if defined (WIN32) || (_WIN32)
    #include <d3d10.h>
#endif //WIN32

#include "D3D10RenderTarget.h"

struct D3D10Context
{
    ID3D10Device *              Device = nullptr;
    IDXGISwapChain *            SwapChain = nullptr;
    D3D10RenderTarget *             DefaultRT = nullptr;
    ID3D10Texture2D *           DepthStencilBuffer = nullptr;
    ID3D10DepthStencilView *    DepthStencilView = nullptr;

    ID3D10DepthStencilState *   dsStateZOn = nullptr;
    ID3D10DepthStencilState *   dsStateZOff = nullptr;

    ID3D10RasterizerState *     rsWireframe = nullptr;
    ID3D10RasterizerState *     rsNormal = nullptr;
    ID3D10RasterizerState *     rsNoCulling = nullptr;

    ID3D10BlendState *          bsAlpha = nullptr;
    ID3D10BlendState *          bsNormal = nullptr;
    ID3D10BlendState *          bsAdditive = nullptr;

    D3D10Context(const D3D10Context & ref) = delete;
    D3D10Context(D3D10Context && ref) = delete;
    D3D10Context & operator=(const D3D10Context & ref) = delete;
    D3D10Context & operator=(D3D10Context && ref) = delete;

    D3D10Context()
        : DefaultRT(new D3D10RenderTarget(nullptr, nullptr))
    {
    }

    ~D3D10Context()
    {
        if (Device)
            Device->ClearState();
        if (bsNormal)
            bsNormal->Release();
        if (bsAlpha)
            bsAlpha->Release();
        if (bsAdditive)
            bsAdditive->Release();
        if (dsStateZOn)
            dsStateZOn->Release();
        if (dsStateZOff)
            dsStateZOff->Release();
        if (rsNormal)
            rsNormal->Release();
        if (rsWireframe)
            rsWireframe->Release();
        if (DepthStencilView)
            DepthStencilView->Release();
        if (DepthStencilBuffer)
            DepthStencilBuffer->Release();
        if (DefaultRT)
            delete DefaultRT;
        //g_SwapChain->Release();
        //g_Device->Release();
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
        if (FAILED(Device->CreateBlendState(&bdDescNormal, &bsNormal)))
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
        if (FAILED(Device->CreateBlendState(&bdDescAlpha, &bsAlpha)))
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
        if (FAILED(Device->CreateBlendState(&bdDescAdditive, &bsAdditive)))
            return false;

        D3D10_DEPTH_STENCIL_DESC dsDescZOn;
        ZeroMemory(&dsDescZOn, sizeof(D3D10_DEPTH_STENCIL_DESC));
        dsDescZOn.DepthEnable = true;
        dsDescZOn.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
        dsDescZOn.DepthFunc = D3D10_COMPARISON_LESS_EQUAL;
        if (FAILED(Device->CreateDepthStencilState(&dsDescZOn, &dsStateZOn)))
            return false;

        D3D10_DEPTH_STENCIL_DESC dsDescZOff;
        ZeroMemory(&dsDescZOff, sizeof(D3D10_DEPTH_STENCIL_DESC));
        dsDescZOff.DepthEnable = false;
        if (FAILED(Device->CreateDepthStencilState(&dsDescZOff, &dsStateZOff)))
            return false;

        D3D10_RASTERIZER_DESC rsDescNormal;
        ZeroMemory(&rsDescNormal, sizeof(D3D10_RASTERIZER_DESC));
        rsDescNormal.FillMode = D3D10_FILL_SOLID;
        rsDescNormal.CullMode = D3D10_CULL_BACK;
        rsDescNormal.FrontCounterClockwise = false;
        rsDescNormal.MultisampleEnable = true;
        if (FAILED(Device->CreateRasterizerState(&rsDescNormal, &rsNormal)))
            return false;

        D3D10_RASTERIZER_DESC rsDescWireframe;
        ZeroMemory(&rsDescWireframe, sizeof(D3D10_RASTERIZER_DESC));
        rsDescWireframe.FillMode = D3D10_FILL_WIREFRAME;
        rsDescWireframe.CullMode = D3D10_CULL_NONE;
        rsDescWireframe.FrontCounterClockwise = false;
        rsDescWireframe.MultisampleEnable = true;
        if (FAILED(Device->CreateRasterizerState(&rsDescWireframe, &rsWireframe)))
            return false;

        D3D10_RASTERIZER_DESC rsDescNoCulling;
        ZeroMemory(&rsDescNoCulling, sizeof(D3D10_RASTERIZER_DESC));
        rsDescNoCulling.FillMode = D3D10_FILL_SOLID;
        rsDescNoCulling.CullMode = D3D10_CULL_NONE;
        rsDescNoCulling.FrontCounterClockwise = false;
        rsDescNoCulling.MultisampleEnable = true;
        if (FAILED(Device->CreateRasterizerState(&rsDescNoCulling, &rsNoCulling)))
            return false;

        return true;
    }
};
