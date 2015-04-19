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
        IMeshDataPtr CreateMeshFromVertices(const uint8_t * verticesData, 
                                            size_t verticesDataSize, 
                                            const std::vector<uint32_t> & indicesData, 
                                            const ShiftEngine::VertexSemantic * semantic) override;
        IMeshDataPtr LoadErrorMesh() override;
    private:
        bool                    Load(const std::wstring &filename, D3D11MeshData * mesh);
        void                    UpdateVertices(SerializedLIM & vertices, std::vector<uint32_t> & indices) const;
        const VertexSemantic *  CreateSemantic(const SerializedLIM & vertices);

        IMeshDataPtr errorMesh;
        std::wstring errorName;
        std::map<std::wstring, D3D11MeshDataPtr> meshesData;
        std::set<VertexSemantic> semantics;
        ID3D11Device * pDevice;
    };
}
