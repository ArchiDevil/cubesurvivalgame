#include "ShiftEngine.h"

#include "cMeshLoader.h"

using namespace std;

ShiftEngine::cMeshLoader::cMeshLoader( ID3D10Device * _device )
	: pDevice(_device)
{
}

bool ShiftEngine::cMeshLoader::Load( std::wstring filename, MeshData * mesh )
{
	ShiftEngine::DefaultVertex * vertices = nullptr;
	long * indices = nullptr;

	if(!LoadFromFile(filename, mesh, &vertices, &indices))
	{
		return false;
	}
	else
	{
		UpdateVertices(mesh, vertices, indices);
		if(!mesh->CreateBuffers(true, vertices, mesh->verticesCount * sizeof(ShiftEngine::DefaultVertex),
			indices, sizeof(long) * mesh->indicesCount, pDevice))
		{
			delete [] vertices;
			delete [] indices;
			return false;
		}
		delete [] vertices;
		delete [] indices;
	}
	return true;
}

bool ShiftEngine::cMeshLoader::LoadFromFile( std::wstring filename, MeshData * mesh, ShiftEngine::DefaultVertex ** vertices, long ** indices )
{
	mesh->vertexSize = sizeof(ShiftEngine::DefaultVertex);
	mesh->vertexSemantic = &ShiftEngine::defaultVertexSemantic;
	mesh->vertexDeclaration = ShiftEngine::GetContextManager()->GetVertexDeclaration(defaultVertexSemantic);
	return ShiftEngine::Utilities::getVerticesFromFile(filename, vertices, &mesh->verticesCount, indices, &mesh->indicesCount);
}

void ShiftEngine::cMeshLoader::UpdateVertices( MeshData * mesh, ShiftEngine::DefaultVertex * vertices, long * indices )
{
	for (unsigned int i = 0; i < mesh->verticesCount ; i++)
	{
		vertices[i].Pos[0] *= (-1);					//X * -1
		float tempA = vertices[i].Pos[1];
		vertices[i].Pos[1] = vertices[i].Pos[2];
		vertices[i].Pos[2] = tempA;					//swapping Y <-> Z

		vertices[i].Normal[0] *= (-1);					//nX * -1
		tempA = vertices[i].Normal[1];
		vertices[i].Normal[1] = vertices[i].Normal[2];
		vertices[i].Normal[2] = tempA;					//swapping nY <-> nZ
	}

	for (unsigned int i = 0; i < mesh->indicesCount; i += 3)
	{
		int a = indices[i+1];						//swapping 0 <-> 1 indices
		indices[i+1] = indices[i];
		indices[i] = a;
	}
}