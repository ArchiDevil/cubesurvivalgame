#pragma once

#include <D3D11.h>

#include <string>
#include <map>
#include <set>

#include "../../IMeshManager.h"
#include "D3D11Mesh.h"

namespace ShiftEngine
{
    class D3D11MeshManager : public IMeshManager
    {
    public:
        D3D11MeshManager(ID3D11Device * _device);
        IMeshDataPtr LoadMesh(const std::wstring & fileName) override;
        IMeshDataPtr CreateMeshFromVertices(const void * verticesData,
                                                       size_t verticesCount,
                                                       const void * indexData,
                                                       size_t indicesCount,
                                                       const ShiftEngine::VertexSemantic * semantic,
                                                       const D3D11VDPtr & declaration) override;
        IMeshDataPtr LoadErrorMesh() override;
    private:
        void                    CreateErrorMesh();
        bool                    Load(const std::wstring &filename, D3D11MeshData * mesh);
        void                    UpdateVertices(SerializedLIM & vertices, std::vector<unsigned long> & indices) const;
        const VertexSemantic *  CreateSemantic(const SerializedLIM & vertices);

        D3D11MeshDataPtr errorMesh;
        std::wstring errorName;
        std::map<std::wstring, D3D11MeshDataPtr> meshesData;
        std::set<VertexSemantic> semantics;
        ID3D11Device * device;
    };
}
