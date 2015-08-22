#pragma once

#include "IMeshData.h"

namespace ShiftEngine
{
    class IMeshManager
    {
    public:
        virtual ~IMeshManager() {}
        virtual IMeshDataPtr LoadMesh(const std::wstring & fileName) = 0;
        virtual IMeshDataPtr CreateMeshFromVertices(const uint8_t * verticesData, 
                                                    size_t verticesDataSize, 
                                                    const std::vector<uint32_t> & indicesData, 
                                                    const ShiftEngine::VertexSemantic * semantic,
                                                    const MathLib::AABB & bbox) = 0;
        virtual IMeshDataPtr LoadErrorMesh() = 0;
    };
}
