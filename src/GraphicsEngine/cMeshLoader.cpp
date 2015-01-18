#include "cMeshLoader.h"

#include "ShiftEngine.h"

#include <memory>
#include <fstream>

using namespace std;

ShiftEngine::cMeshLoader::cMeshLoader(ID3D10Device * _device)
	: pDevice(_device)
{
}

bool ShiftEngine::cMeshLoader::Load(const std::wstring &filename, MeshData * mesh)
{
	SerializedLIM vertices;
	std::vector<unsigned long> indices;

	if (!ShiftEngine::Utilities::getVerticesFromFile(filename, vertices, indices))
		return false;

	UpdateVertices(vertices, indices);

	std::vector<uint8_t> vertexData;
	size_t resultSize = vertices.position.size() * sizeof(Vector3F) +
						vertices.normal.size() * sizeof(Vector3F) +
						vertices.texcoord.size() * sizeof(Vector2F) +
						vertices.colors.size() * sizeof(Vector3F);

	size_t vertexSize = sizeof(Vector3F);
	if (!vertices.normal.empty())
		vertexSize += sizeof(Vector3F);
	if (!vertices.texcoord.empty())
		vertexSize += sizeof(Vector2F);
	if (!vertices.colors.empty())
		vertexSize += sizeof(Vector3F);

	vertexData.resize(resultSize);

	uint8_t *pData = vertexData.data();
	for (size_t i = 0; i < vertices.position.size(); ++i)
	{
		memcpy(pData, &(vertices.position[i]), sizeof(Vector3F));
		pData += sizeof(Vector3F);
		if (vertices.normal.size() > 0)
		{
			memcpy(pData, &(vertices.normal[i]), sizeof(Vector3F));
			pData += sizeof(Vector3F);
		}

		if (vertices.texcoord.size() > 0)
		{
			memcpy(pData, &(vertices.texcoord[i]), sizeof(Vector2F));
			pData += sizeof(Vector2F);
		}

		if (vertices.colors.size() > 0)
		{
			memcpy(pData, &(vertices.colors[i]), sizeof(Vector3F));
			pData += sizeof(Vector3F);
		}
	}

	mesh->verticesCount = vertices.position.size();
	mesh->indicesCount = indices.size();
	mesh->vertexSize = vertexSize;
	mesh->vertexSemantic = CreateSemantic(vertices);
	auto declaration = ShiftEngine::GetContextManager()->GetVertexDeclaration(*mesh->vertexSemantic);
	if (!declaration)
	{
		GetContextManager()->RegisterVertexSemantic(*mesh->vertexSemantic);
		declaration = GetContextManager()->GetVertexDeclaration(*mesh->vertexSemantic);
	}
	mesh->vertexDeclaration = declaration;

	if (!mesh->CreateBuffers(true, vertexData.data(), vertices.position.size() * vertexSize, indices.data(), indices.size() * sizeof(unsigned long), pDevice))
		return false;

	return true;
}

void ShiftEngine::cMeshLoader::UpdateVertices(SerializedLIM & vertices, std::vector<unsigned long> & indices) const
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

const ShiftEngine::VertexSemantic * ShiftEngine::cMeshLoader::CreateSemantic(const SerializedLIM & vertices)
{
	VertexSemantic sem;
	sem.addSemantic(ET_FLOAT, 3, ES_Position);
	if(!vertices.normal.empty())
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
	return nullptr;
}
