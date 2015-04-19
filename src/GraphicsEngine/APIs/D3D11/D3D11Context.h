#pragma once

#include <d3d11.h>

#include "D3D11RenderTarget.h"

struct D3D11Context
{
    ID3D11Device *                      Device = nullptr;
    ID3D11DeviceContext *               DeviceContext = nullptr;
    IDXGISwapChain *                    SwapChain = nullptr;
    std::unique_ptr<D3D11RenderTarget>  DefaultRT = nullptr;
    ID3D11Texture2D *                   DepthStencilBuffer = nullptr;
    ID3D11DepthStencilView *            DepthStencilView = nullptr;

    ID3D11DepthStencilState *           dsStateZOn = nullptr;
    ID3D11DepthStencilState *           dsStateZOff = nullptr;

    ID3D11RasterizerState *             rsWireframe = nullptr;
    ID3D11RasterizerState *             rsNormal = nullptr;
    ID3D11RasterizerState *             rsNoCulling = nullptr;

    ID3D11BlendState *                  bsAlpha = nullptr;
    ID3D11BlendState *                  bsNormal = nullptr;
    ID3D11BlendState *                  bsAdditive = nullptr;

    D3D11Context(const D3D11Context & ref) = delete;
    D3D11Context(D3D11Context && ref) = delete;
    D3D11Context & operator=(const D3D11Context & ref) = delete;
    D3D11Context & operator=(D3D11Context && ref) = delete;

    D3D11Context()
        : DefaultRT(new D3D11RenderTarget(nullptr, nullptr))
    {
    }

    ~D3D11Context()
    {
        if (DeviceContext)
            DeviceContext->ClearState();
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
        DefaultRT.reset(nullptr);
        DeviceContext->Release();
        SwapChain->Release();
        Device->Release();
    }

    HRESULT CreateStates()
    {
        D3D11_BLEND_DESC bdDescNormal;
        ZeroMemory(&bdDescNormal, sizeof(D3D11_BLEND_DESC));
        bdDescNormal.AlphaToCoverageEnable = false;
        for (int i = 0; i < 8; ++i)
        {
            bdDescNormal.RenderTarget[i].BlendEnable = false;
            bdDescNormal.RenderTarget[i].BlendOp = D3D11_BLEND_OP_MAX;
            bdDescNormal.RenderTarget[i].DestBlend = D3D11_BLEND_DEST_COLOR;
            bdDescNormal.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_COLOR;
            bdDescNormal.RenderTarget[i].RenderTargetWriteMask = 0x0F;
        }
        HRESULT hr = Device->CreateBlendState(&bdDescNormal, &bsNormal);
        if (FAILED(hr))
            return hr;

        D3D11_BLEND_DESC bdDescAlpha;
        ZeroMemory(&bdDescAlpha, sizeof(D3D11_BLEND_DESC));
        bdDescAlpha.AlphaToCoverageEnable = false;
        for (int i = 0; i < 8; ++i)
        {
            bdDescAlpha.RenderTarget[i].BlendEnable = true;
            bdDescAlpha.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
            bdDescAlpha.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            bdDescAlpha.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            bdDescAlpha.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            bdDescAlpha.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
            bdDescAlpha.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
            bdDescAlpha.RenderTarget[i].RenderTargetWriteMask = 0x0F;
        }
        hr = Device->CreateBlendState(&bdDescAlpha, &bsAlpha);
        if (FAILED(hr))
            return hr;

        D3D11_BLEND_DESC bdDescAdditive;
        ZeroMemory(&bdDescAdditive, sizeof(D3D11_BLEND_DESC));
        bdDescAdditive.AlphaToCoverageEnable = false;
        for (int i = 0; i < 8; ++i)
        {
            bdDescAdditive.RenderTarget[i].BlendEnable = true;
            bdDescAdditive.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
            bdDescAdditive.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            bdDescAdditive.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
            bdDescAdditive.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
            bdDescAdditive.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
            bdDescAdditive.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
            bdDescAdditive.RenderTarget[i].RenderTargetWriteMask = 0x0F;
        }
        hr = Device->CreateBlendState(&bdDescAdditive, &bsAdditive);
        if (FAILED(hr))
            return hr;

        D3D11_DEPTH_STENCIL_DESC dsDescZOn;
        ZeroMemory(&dsDescZOn, sizeof(D3D11_DEPTH_STENCIL_DESC));
        dsDescZOn.DepthEnable = true;
        dsDescZOn.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDescZOn.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        hr = Device->CreateDepthStencilState(&dsDescZOn, &dsStateZOn);
        if (FAILED(hr))
            return hr;

        D3D11_DEPTH_STENCIL_DESC dsDescZOff;
        ZeroMemory(&dsDescZOff, sizeof(D3D11_DEPTH_STENCIL_DESC));
        dsDescZOff.DepthEnable = false;
        hr = Device->CreateDepthStencilState(&dsDescZOff, &dsStateZOff);
        if (FAILED(hr))
            return hr;

        D3D11_RASTERIZER_DESC rsDescNormal;
        ZeroMemory(&rsDescNormal, sizeof(D3D11_RASTERIZER_DESC));
        rsDescNormal.FillMode                   = D3D11_FILL_SOLID;
        rsDescNormal.CullMode                   = D3D11_CULL_BACK;
        rsDescNormal.FrontCounterClockwise      = FALSE;
        rsDescNormal.MultisampleEnable          = TRUE;
        rsDescNormal.DepthClipEnable            = TRUE;
        hr = Device->CreateRasterizerState(&rsDescNormal, &rsNormal);
        if (FAILED(hr))
            return false;

        D3D11_RASTERIZER_DESC rsDescWireframe;
        ZeroMemory(&rsDescWireframe, sizeof(D3D11_RASTERIZER_DESC));
        rsDescWireframe.FillMode                = D3D11_FILL_WIREFRAME;
        rsDescWireframe.CullMode                = D3D11_CULL_NONE;
        rsDescWireframe.FrontCounterClockwise   = FALSE;
        rsDescWireframe.MultisampleEnable       = TRUE;
        rsDescWireframe.DepthClipEnable = TRUE;
        hr = Device->CreateRasterizerState(&rsDescWireframe, &rsWireframe);
        if (FAILED(hr))
            return hr;

        D3D11_RASTERIZER_DESC rsDescNoCulling;
        ZeroMemory(&rsDescNoCulling, sizeof(D3D11_RASTERIZER_DESC));
        rsDescNoCulling.FillMode                = D3D11_FILL_SOLID;
        rsDescNoCulling.CullMode                = D3D11_CULL_NONE;
        rsDescNoCulling.FrontCounterClockwise   = FALSE;
        rsDescNoCulling.MultisampleEnable       = TRUE;
        rsDescNoCulling.DepthClipEnable = TRUE;
        hr = Device->CreateRasterizerState(&rsDescNoCulling, &rsNoCulling);
        if (FAILED(hr))
            return hr;

        return true;
    }
};
