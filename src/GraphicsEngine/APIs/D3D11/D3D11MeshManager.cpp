#include "D3D11MeshManager.h"

#include <Utilities/logger.hpp>
#include <Utilities/ut.h>

#include "../../ShiftEngine.h"
#include "../../Utils.h"

ShiftEngine::D3D11MeshManager::D3D11MeshManager(ID3D11Device * _device)
    : device(_device)
{
    CreateErrorMesh();
}

ShiftEngine::IMeshDataPtr ShiftEngine::D3D11MeshManager::LoadMesh(const std::wstring & fileName)
{
    auto iter = meshesData.find(fileName);
    if (iter != meshesData.end())
    {
        return iter->second;
    }
    else
    {
        D3D11MeshData * ptr = new D3D11MeshData;
        if (!Load(fileName, ptr))
        {
            delete ptr;
            LOG_ERROR("Unable to load mesh", utils::Narrow(fileName));
            return nullptr;
        }
        else
        {
            meshesData[fileName] = D3D11MeshDataPtr(ptr);
            return D3D11MeshDataPtr(ptr);
        }
    }
}

ShiftEngine::IMeshDataPtr ShiftEngine::D3D11MeshManager::CreateMeshFromVertices(const uint8_t * verticesData, size_t verticesDataSize, const std::vector<uint32_t> & indicesData, const ShiftEngine::VertexSemantic * semantic)
{
    D3D11MeshDataPtr out = std::make_shared<D3D11MeshData>();
    if (!out->CreateBuffers(false, verticesData, verticesDataSize, indicesData.data(), indicesData.size()))
    {
        LOG_ERROR("Unable to create mesh from vertices");
        return nullptr;
    }
    return out;
}

ShiftEngine::IMeshDataPtr ShiftEngine::D3D11MeshManager::LoadErrorMesh()
{
    return errorMesh;
}

bool ShiftEngine::D3D11MeshManager::Load(const std::wstring & filename, D3D11MeshData * mesh)
{
    SerializedLIM vertices;
    std::vector<uint32_t> indices;

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

    if (!mesh->CreateBuffers(true, vertexData.data(), vertices.position.size() * vertexSize, indices.data(), indices.size() * sizeof(unsigned long)))
        return false;

    return true;
}

void ShiftEngine::D3D11MeshManager::UpdateVertices(SerializedLIM & vertices, std::vector<unsigned long>& indices) const
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

const ShiftEngine::VertexSemantic * ShiftEngine::D3D11MeshManager::CreateSemantic(const SerializedLIM & vertices)
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
