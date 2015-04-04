#pragma once

#include <d3d11.h>

namespace ShiftEngine
{
    class D3D11VertexDeclaration
    {
    public:
        D3D11VertexDeclaration(ID3D10InputLayout * _IL = nullptr, ID3D10Device * _pDevice = nullptr)
            : IL(_IL)
            , pDevice(_pDevice)
        {
        }

        D3D11VertexDeclaration(const D3D11VertexDeclaration & ref)
        {
            this->IL = ref.IL;
            this->pDevice = ref.pDevice;
            if (IL) IL->AddRef();
        }

        ~D3D11VertexDeclaration()
        {
            if (IL) IL->Release();
        }

        void Bind()
        {
            if (IL)
                pDevice->IASetInputLayout(this->IL);
        }

        ID3D11InputLayout * IL = nullptr;
        ID3D11Device * pDevice = nullptr;
    };

    typedef std::shared_ptr<D3D11VertexDeclaration> D3D11VDPtr;
}
