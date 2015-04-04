#pragma once

#include <D3D10.h>

#include <string>
#include <map>
#include <set>

#include "cMesh.h"

namespace ShiftEngine
{
    class D3D10MeshManager
    {
    public:
        D3D10MeshManager(ID3D10Device * _device);
        MeshDataPtr LoadMesh(const std::wstring & fileName);
        MeshDataPtr CreateMeshFromVertices(const void * verticesData,
                                           size_t verticesCount,
                                           const void * indexData,
                                           size_t indicesCount,
                                           const ShiftEngine::VertexSemantic * semantic,
                                           const D3D10VDPtr & declaration);
        MeshDataPtr LoadErrorMesh();
    private:
        void        CreateErrorMesh();

        bool Load(const std::wstring &filename, MeshData * mesh);
        void UpdateVertices(SerializedLIM & vertices, std::vector<unsigned long> & indices) const;
        const VertexSemantic * CreateSemantic(const SerializedLIM & vertices);

        MeshDataPtr errorMesh;
        std::wstring errorName;
        std::map<std::wstring, MeshDataPtr> meshesData;
        std::set<VertexSemantic> semantics;
        ID3D10Device * device;
    };
}
