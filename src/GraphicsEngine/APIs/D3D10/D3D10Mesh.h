#pragma once

#include <d3d10.h>
#include <string>
#include <memory>

#include "D3D10VertexDeclaration.h"

#include "../../IMeshData.h"
#include "../../VertexTypes.h"
#include "../../MiscTypes.h"

namespace ShiftEngine
{
    //mesh must contain AABB
    class D3D10MeshData : public IMeshData
    {
    public:
        D3D10MeshData(ID3D10Device * pDevice);
        D3D10MeshData(ID3D10Buffer * _VB, ID3D10Buffer * _IB, ID3D10Device * pDevice);
        D3D10MeshData(const D3D10MeshData & ref);
        D3D10MeshData& operator = (const D3D10MeshData & ref);
        ~D3D10MeshData();

        bool CreateBuffers(bool dynamic,
            const uint8_t * vData,
            size_t vDataSize,
            const uint32_t * iData,
            size_t iDataSize,
            const VertexSemantic * semantic,
            const D3D10VDPtr & declaration) override;

        size_t Draw() override;
        void Clear() override;

    private:
        ID3D10Device * pDevice = nullptr;
        ID3D10Buffer * VertexBuffer = nullptr;
        ID3D10Buffer * IndexBuffer = nullptr;
    };

    typedef std::shared_ptr<D3D10MeshData> D3D10MeshDataPtr;
}
