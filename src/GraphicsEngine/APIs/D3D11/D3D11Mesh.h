#pragma once

#include <d3d11.h>
#include <string>
#include <memory>

#include "D3D11VertexDeclaration.h"

#include "../../IMeshData.h"
#include "../../VertexTypes.h"
#include "../../MiscTypes.h"

namespace ShiftEngine
{
    //mesh must contain AABB
    class D3D11MeshData : public IMeshData
    {
    public:
        D3D11MeshData(ID3D11Buffer * _VB = nullptr, ID3D11Buffer * _IB = nullptr, 
                      ID3D11Device * pDevice = nullptr, ID3D11DeviceContext * pDeviceContext = nullptr);
        D3D11MeshData(const D3D11MeshData & ref);
        D3D11MeshData& operator = (const D3D11MeshData & ref);
        ~D3D11MeshData();

        bool CreateBuffers(bool dynamic, const uint8_t * vData, size_t vDataSize, const uint32_t * iData, size_t iDataSize, const VertexSemantic * semantic, const IVertexDeclarationPtr & declaration) override;
        size_t Draw() override;
        void Clear() override;

    private:
        ID3D11Device * pDevice = nullptr;
        ID3D11DeviceContext * pDeviceContext = nullptr;
        ID3D11Buffer * VertexBuffer = nullptr;
        ID3D11Buffer * IndexBuffer = nullptr;
    };

    typedef std::shared_ptr<D3D11MeshData> D3D11MeshDataPtr;
}
