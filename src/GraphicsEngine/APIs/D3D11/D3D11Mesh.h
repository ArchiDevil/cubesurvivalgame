#pragma once

#include <d3d11.h>
#include <string>
#include <memory>

#include "D3D11VertexDeclaration.h"

#include "../../VertexTypes.h"
#include "../../MiscTypes.h"

namespace ShiftEngine
{
    //mesh must contain AABB
    struct D3D11MeshData
    {
        D3D11MeshData(D3D11VDPtr _vertexDeclaration = nullptr, ID3D11Buffer * _VB = nullptr, ID3D11Buffer * _IB = nullptr, 
                      ID3D11Device * pDevice = nullptr, ID3D11DeviceContext * pDeviceContext = nullptr);
        D3D11MeshData(const D3D11MeshData & ref);
        D3D11MeshData& operator = (const D3D11MeshData & ref);
        ~D3D11MeshData();

        bool CreateBuffers(bool dynamic, const void * vData, size_t vSize,
                           const void * iData, size_t iSize);
        int Draw();
        void Clear();

        const VertexSemantic * vertexSemantic;
        D3D11VDPtr vertexDeclaration;
        ID3D11Device * pDevice = nullptr;
        ID3D11DeviceContext * pDeviceContext = nullptr;
        ID3D11Buffer * VertexBuffer = nullptr;
        ID3D11Buffer * IndexBuffer = nullptr;
        size_t verticesCount = 0;
        size_t indicesCount = 0;
        size_t vertexSize = 0;
    };

    typedef std::shared_ptr<D3D11MeshData> D3D11MeshDataPtr;
}
