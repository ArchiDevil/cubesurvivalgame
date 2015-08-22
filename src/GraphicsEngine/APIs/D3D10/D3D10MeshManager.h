#pragma once

#include <D3D10.h>

#include <string>
#include <map>
#include <set>

#include "D3D10Mesh.h"
#include "../../IMeshManager.h"

namespace ShiftEngine
{
    class D3D10MeshManager : public IMeshManager
    {
    public:
        D3D10MeshManager(ID3D10Device * _device);
        IMeshDataPtr            LoadMesh(const std::wstring & fileName) override;
        IMeshDataPtr            CreateMeshFromVertices(const uint8_t * verticesData,
                                                       size_t verticesDataSize,
                                                       const std::vector<uint32_t> & indicesData,
                                                       const ShiftEngine::VertexSemantic * semantic,
                                                       const MathLib::AABB & bbox) override;
        IMeshDataPtr            LoadErrorMesh() override;

    private:
        bool                    Load(const std::wstring &filename, D3D10MeshData * mesh);
        void                    UpdateVertices(SerializedLIM & vertices, std::vector<uint32_t> & indices) const;
        const VertexSemantic *  CreateSemantic(const SerializedLIM & vertices);

        IMeshDataPtr errorMesh;
        std::wstring errorName;
        std::map<std::wstring, D3D10MeshDataPtr> meshesData;
        std::set<VertexSemantic> semantics;
        ID3D10Device * device;
    };
}
