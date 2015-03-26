#pragma once

#include <d3d10.h>

namespace ShiftEngine
{
    class D3D10VertexDeclaration
    {
    public:
        D3D10VertexDeclaration(ID3D10InputLayout * _IL = nullptr, ID3D10Device * _pDevice = nullptr)
            : IL(_IL)
            , pDevice(_pDevice)
        {
        }

        D3D10VertexDeclaration(const D3D10VertexDeclaration & ref)
        {
            this->IL = ref.IL;
            this->pDevice = ref.pDevice;
            if (IL) IL->AddRef();
        }

        ~D3D10VertexDeclaration()
        {
            if (IL) IL->Release();
        }

        void Bind()
        {
            if (IL)
                pDevice->IASetInputLayout(this->IL);
        }

        ID3D10InputLayout * IL = nullptr;
        ID3D10Device * pDevice = nullptr;
    };

    typedef std::shared_ptr<D3D10VertexDeclaration> D3D10VDPtr;
}
