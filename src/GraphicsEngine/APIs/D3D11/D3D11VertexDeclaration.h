#pragma once

#include <d3d11.h>
#include <memory>

#include "../../IVertexDeclaration.h"

namespace ShiftEngine
{
    class D3D11VertexDeclaration : public IVertexDeclaration
    {
    public:
        D3D11VertexDeclaration(ID3D11InputLayout * IL = nullptr, ID3D11DeviceContext * pDeviceContext = nullptr)
            : IL(IL)
            , pDeviceContext(pDeviceContext)
        {
        }

        D3D11VertexDeclaration(const D3D11VertexDeclaration & ref)
        {
            IL = ref.IL;
            pDeviceContext = ref.pDeviceContext;

            if (IL) 
                IL->AddRef();
        }

        ~D3D11VertexDeclaration()
        {
            if (IL) 
                IL->Release();
        }

        void Bind() override
        {
            if (IL)
                pDeviceContext->IASetInputLayout(this->IL);
        }

        ID3D11InputLayout * IL = nullptr;
        ID3D11DeviceContext * pDeviceContext = nullptr;
    };

    typedef std::shared_ptr<D3D11VertexDeclaration> D3D11VertexDeclarationPtr;
}
