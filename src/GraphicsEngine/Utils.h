#pragma once

#include "VertexTypes.h"
#include "MiscTypes.h"
#include "cMesh.h"

namespace ShiftEngine
{
    namespace Utilities
    {
        bool getVerticesFromFile(const std::wstring & filename,
            SerializedLIM & vertices,
            std::vector<unsigned long> & indices);

        MathLib::AABB getBBoxFromMesh(MeshDataPtr meshData);

        MeshDataPtr createCube();
        MeshDataPtr createPlane();
        MeshDataPtr createSphere();

        void convertVertices(const std::vector<DefaultVertex> & input, std::vector<ExtendedVertex> & output);
        void convertVertices(const std::vector<DefaultVertex> & input, std::vector<ColorVertex> & output);
        void convertVertices(const std::vector<ExtendedVertex> & input, std::vector<DefaultVertex> & output);
        void convertVertices(const std::vector<ExtendedVertex> & input, std::vector<ColorVertex> & output);
        void convertVertices(const std::vector<ColorVertex> & input, std::vector<DefaultVertex> & output);
        void convertVertices(const std::vector<ColorVertex> & input, std::vector<ExtendedVertex> & output);
    }
}
