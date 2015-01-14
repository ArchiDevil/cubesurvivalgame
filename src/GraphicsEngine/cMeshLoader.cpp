#include "cMeshLoader.h"

#include "ShiftEngine.h"

#include <memory>

using namespace std;

ShiftEngine::cMeshLoader::cMeshLoader(ID3D10Device * _device)
    : pDevice(_device)
{
}

bool ShiftEngine::cMeshLoader::Load(std::wstring filename, MeshData * mesh)
{
    std::unique_ptr<DefaultVertex[]> vertices = nullptr;
    std::unique_ptr<long[]> indices = nullptr;

    if (!LoadFromFile(filename, mesh, vertices, indices))
    {
        return false;
    }

    UpdateVertices(mesh, vertices.get(), indices.get());
    if (!mesh->CreateBuffers(true, 
                            vertices.get(), 
                            mesh->verticesCount * sizeof(ShiftEngine::DefaultVertex),
                            indices.get(), 
                            sizeof(long) * mesh->indicesCount, pDevice))
    {
        return false;
    }

    return true;
}

bool ShiftEngine::cMeshLoader::LoadFromFile(std::wstring filename, MeshData * mesh, std::unique_ptr<DefaultVertex[]> & vertices, std::unique_ptr<long[]> & indices)
{
    mesh->vertexSize = sizeof(ShiftEngine::DefaultVertex);
    mesh->vertexSemantic = &ShiftEngine::defaultVertexSemantic;
    mesh->vertexDeclaration = ShiftEngine::GetContextManager()->GetVertexDeclaration(defaultVertexSemantic);
    std::unique_ptr<DefaultVertex> local_vertices = nullptr;
    std::unique_ptr<long> local_indices = nullptr;
    return ShiftEngine::Utilities::getVerticesFromFile(filename, vertices, &mesh->verticesCount, indices, &mesh->indicesCount);
}

void ShiftEngine::cMeshLoader::UpdateVertices(MeshData * mesh, ShiftEngine::DefaultVertex * vertices, long * indices)
{
    for (size_t i = 0; i < mesh->verticesCount; i++)
    {
        vertices[i].Pos[0] *= -1;					//X * -1
        std::swap(vertices[i].Pos[1], vertices[i].Pos[2]);

        vertices[i].Normal[0] *= -1;				//nX * -1
        std::swap(vertices[i].Normal[1], vertices[i].Normal[1]);
    }

    for (size_t i = 0; i < mesh->indicesCount; i += 3)
        std::swap(indices[i], indices[i + 1]);
}
