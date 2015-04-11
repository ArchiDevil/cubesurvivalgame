#pragma once

#include "VertexTypes.h"
#include "MiscTypes.h"
#include "APIs/D3D10/D3D10Mesh.h"

namespace ShiftEngine
{
    namespace Utilities
    {
        bool getVerticesFromFile(const std::wstring & filename,
                                 SerializedLIM & vertices,
                                 std::vector<uint32_t> & indices);

        MathLib::AABB getBBoxFromMesh(D3D10MeshDataPtr meshData);

        IMeshDataPtr createCube();
        IMeshDataPtr createPlane();
        IMeshDataPtr createSphere();

        void convertVertices(const std::vector<DefaultVertex> & input, std::vector<ExtendedVertex> & output);
        void convertVertices(const std::vector<DefaultVertex> & input, std::vector<ColorVertex> & output);
        void convertVertices(const std::vector<ExtendedVertex> & input, std::vector<DefaultVertex> & output);
        void convertVertices(const std::vector<ExtendedVertex> & input, std::vector<ColorVertex> & output);
        void convertVertices(const std::vector<ColorVertex> & input, std::vector<DefaultVertex> & output);
        void convertVertices(const std::vector<ColorVertex> & input, std::vector<ExtendedVertex> & output);
    }
}
