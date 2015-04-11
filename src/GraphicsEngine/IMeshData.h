#pragma once

#include <string>
#include <memory>

#include "APIs/D3D10/D3D10VertexDeclaration.h"

#include "VertexTypes.h"
#include "MiscTypes.h"

namespace ShiftEngine
{
    //mesh must contain AABB
    class IMeshData
    {
    public:
        virtual ~IMeshData() {}
        virtual uint32_t Draw() = 0;
        virtual void Clear() = 0;

        size_t GetVerticesCount() const { return verticesCount; }
        size_t GetIndicesCount() const { return indicesCount; }
        size_t GetVertexSize() const { return vertexSize; }
        const VertexSemantic * GetVertexSemantic() const { return vertexSemantic; }
        ShiftEngine::D3D10VDPtr GetVertexDeclaration() const { return vertexDeclaration; }

        virtual bool CreateBuffers(bool dynamic,
            const uint8_t * vData,
            size_t vDataSize,
            const uint32_t * iData,
            size_t iDataSize,
            const VertexSemantic * semantic,
            const D3D10VDPtr & declaration) = 0;

        IMeshData() = default;
        IMeshData(IMeshData & other) = default;
        IMeshData & operator=(IMeshData & other) = default;

    protected:
        const VertexSemantic * vertexSemantic = nullptr;
        size_t verticesCount = 0;
        size_t indicesCount = 0;
        size_t vertexSize = 0;
        D3D10VDPtr vertexDeclaration = nullptr;
    };

    typedef std::shared_ptr<IMeshData> IMeshDataPtr;
}
