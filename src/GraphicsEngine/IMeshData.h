#pragma once

#include <string>
#include <memory>

#include "IVertexDeclaration.h"

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
        ShiftEngine::IVertexDeclarationPtr GetVertexDeclaration() const { return vertexDeclaration; }
        MathLib::AABB GetBBox() const { return bbox; }

        virtual bool CreateBuffers(bool dynamic,
                                   const uint8_t * vData,
                                   size_t vDataSize,
                                   const uint32_t * iData,
                                   size_t iDataSize,
                                   const VertexSemantic * semantic,
                                   const IVertexDeclarationPtr & declaration,
                                   const MathLib::AABB & bbox) = 0;

        IMeshData() = default;
        IMeshData(IMeshData & other) = default;
        IMeshData & operator=(IMeshData & other) = default;

    protected:
        const VertexSemantic * vertexSemantic = nullptr;
        size_t verticesCount = 0;
        size_t indicesCount = 0;
        size_t vertexSize = 0;
        IVertexDeclarationPtr vertexDeclaration = nullptr;
        MathLib::AABB bbox = {};
    };

    typedef std::shared_ptr<IMeshData> IMeshDataPtr;
}
