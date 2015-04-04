#include "D3D10MeshManager.h"

#include <Utilities/logger.hpp>
#include <Utilities/ut.h>

#include "ShiftEngine.h"
#include "Utils.h"

ShiftEngine::D3D10MeshManager::D3D10MeshManager(ID3D10Device * _device)
    : device(_device)
{
    CreateErrorMesh();
}

ShiftEngine::MeshDataPtr ShiftEngine::D3D10MeshManager::LoadMesh(const std::wstring & fileName)
{
    auto iter = meshesData.find(fileName);
    if (iter != meshesData.end())
    {
        return iter->second;
    }
    else
    {
        MeshData * ptr = new MeshData;
        if (!Load(fileName, ptr))
        {
            delete ptr;
            LOG_ERROR("Unable to load mesh", utils::Narrow(fileName));
            return nullptr;
        }
        else
        {
            meshesData[fileName] = MeshDataPtr(ptr);
            return MeshDataPtr(ptr);
        }
    }
}

ShiftEngine::MeshDataPtr ShiftEngine::D3D10MeshManager::CreateMeshFromVertices(const void * data, 
                                                                            size_t count,
                                                                            const void * indexData, 
                                                                            size_t indicesCount,
                                                                            const ShiftEngine::VertexSemantic * semantic,
                                                                            const D3D10VDPtr & declaration)
{
    MeshDataPtr out = std::make_shared<MeshData>(declaration);
    if (!out->CreateBuffers(false, data, count, indexData, indicesCount, device))
    {
        LOG_ERROR("Unable to create mesh from vertices");
        return nullptr;
    }
    out->vertexSemantic = semantic;
    return out;
}

ShiftEngine::MeshDataPtr ShiftEngine::D3D10MeshManager::LoadErrorMesh()
{
    return errorMesh;
}

void ShiftEngine::D3D10MeshManager::CreateErrorMesh()
{
    errorMesh = Utilities::createCube();
}

bool ShiftEngine::D3D10MeshManager::Load(const std::wstring & filename, MeshData * mesh)
{
    SerializedLIM vertices;
    std::vector<unsigned long> indices;

    if (!ShiftEngine::Utilities::getVerticesFromFile(filename, vertices, indices))
        return false;

    UpdateVertices(vertices, indices);

    std::vector<uint8_t> vertexData;
    size_t resultSize = vertices.position.size() * sizeof(MathLib::Vector3F) +
        vertices.normal.size() * sizeof(MathLib::Vector3F) +
        vertices.texcoord.size() * sizeof(MathLib::Vector2F) +
        vertices.colors.size() * sizeof(MathLib::Vector3F);

    size_t vertexSize = sizeof(MathLib::Vector3F);
    if (!vertices.normal.empty())
        vertexSize += sizeof(MathLib::Vector3F);
    if (!vertices.texcoord.empty())
        vertexSize += sizeof(MathLib::Vector2F);
    if (!vertices.colors.empty())
        vertexSize += sizeof(MathLib::Vector3F);

    vertexData.resize(resultSize);

    uint8_t *pData = vertexData.data();
    for (size_t i = 0; i < vertices.position.size(); ++i)
    {
        memcpy(pData, &(vertices.position[i]), sizeof(MathLib::Vector3F));
        pData += sizeof(MathLib::Vector3F);
        if (vertices.normal.size() > 0)
        {
            memcpy(pData, &(vertices.normal[i]), sizeof(MathLib::Vector3F));
            pData += sizeof(MathLib::Vector3F);
        }

        if (vertices.texcoord.size() > 0)
        {
            memcpy(pData, &(vertices.texcoord[i]), sizeof(MathLib::Vector2F));
            pData += sizeof(MathLib::Vector2F);
        }

        if (vertices.colors.size() > 0)
        {
            memcpy(pData, &(vertices.colors[i]), sizeof(MathLib::Vector3F));
            pData += sizeof(MathLib::Vector3F);
        }
    }

    mesh->verticesCount = vertices.position.size();
    mesh->indicesCount = indices.size();
    mesh->vertexSize = vertexSize;
    mesh->vertexSemantic = CreateSemantic(vertices);
    auto declaration = ShiftEngine::GetContextManager()->GetVertexDeclaration(*mesh->vertexSemantic);
    if (!declaration)
        declaration = GetContextManager()->GetVertexDeclaration(*mesh->vertexSemantic);

    mesh->vertexDeclaration = declaration;

    if (!mesh->CreateBuffers(true, vertexData.data(), vertices.position.size() * vertexSize, indices.data(), indices.size() * sizeof(unsigned long), device))
        return false;

    return true;
}

void ShiftEngine::D3D10MeshManager::UpdateVertices(SerializedLIM & vertices, std::vector<unsigned long>& indices) const
{
    for (size_t i = 0; i < vertices.position.size(); i++)
    {
        vertices.position[i].x *= -1;
        std::swap(vertices.position[i].y, vertices.position[i].z);

        vertices.normal[i].x *= -1;
        std::swap(vertices.normal[i].y, vertices.normal[i].z);
    }

    for (size_t i = 0; i < indices.size(); i += 3)
        std::swap(indices[i], indices[i + 1]);
}

const ShiftEngine::VertexSemantic * ShiftEngine::D3D10MeshManager::CreateSemantic(const SerializedLIM & vertices)
{
    VertexSemantic sem;
    sem.addSemantic(ET_FLOAT, 3, ES_Position);
    if (!vertices.normal.empty())
        sem.addSemantic(ET_FLOAT, 3, ES_Normal);
    if (!vertices.texcoord.empty())
        sem.addSemantic(ET_FLOAT, 2, ES_Texcoord);
    if (!vertices.colors.empty())
        sem.addSemantic(ET_FLOAT, 3, ES_Color);

    auto iter = semantics.find(sem);
    if (iter != semantics.end())
        return &(*iter);

    auto insert_pair = semantics.insert(sem);
    if (insert_pair.second)
    {
        return &(*insert_pair.first);
    }

    // crash I think
    // or return already inserted?
    return nullptr;
}
