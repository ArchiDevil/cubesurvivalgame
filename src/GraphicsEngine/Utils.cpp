#include "Utils.h"

#include <D3DX10.h>
#include "ShiftEngine.h"

#include <fstream>

bool ShiftEngine::Utilities::getVerticesFromFile( const std::wstring & filename, 
											 DefaultVertex ** vertices, 
											 unsigned int * verticesCount, 
											 long ** indices, 
											 unsigned int * indicesCount )
{
	std::ifstream in(filename.c_str(), std::ios::binary);

	if(!in || in.fail())
		return false;

	//стандартный хидер для формата
	MeshLIMHeader head;
	in.read(reinterpret_cast<char*>(&head), sizeof(MeshLIMHeader));
	if(head.VERSION != LIM_HEADER_VERSION)
		return false;

	int size = head.verticesCount;
	*verticesCount = size;

	(*vertices) = new DefaultVertex[head.verticesCount];

	//читаем позицию
	for (int i = 0; i < size ; i++)
		in.read(reinterpret_cast<char*>( ((*vertices)[i]).Pos ), sizeof(float) * 3);

	//нормали
	if(head.hasNormals)
		for (int i = 0; i < size ; i++)
			in.read(reinterpret_cast<char*>( ((*vertices)[i]).Normal ), sizeof(float) * 3);

	//текстурные координаты
	if(head.hasTexCoords)
		for (int i = 0; i < size ; i++)
			in.read(reinterpret_cast<char*>( ((*vertices)[i]).Texcoord ), sizeof(float) * 2);


	//читаем количество индексов из файла
	int sizeInd = head.indicesCount;

	*indices = new long[head.indicesCount];
	*indicesCount = sizeInd;

	//читаем индексы из файла
	in.read(reinterpret_cast<char*>( *indices ), sizeof(long) * sizeInd);
	in.close();
	return true;
}

ShiftEngine::MeshDataPtr ShiftEngine::Utilities::createCube()
{
	//TODO: creating mesh WITHOUT ANY INFO ABOUT API

	ID3D10Device * pDevice = ShiftEngine::GetContextManager()->GetDevicePointer();

	static MeshDataPtr pDefaultMesh = nullptr;
	if(pDefaultMesh != nullptr)
		return pDefaultMesh;
	//some actions
	DefaultVertex ver[] = 
	{
		//up
		{0.5f, 0.5f, 1.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f},
		{0.5f, -0.5f, 1.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f},
		{-0.5f, 0.5f, 1.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f},
		{-0.5f, -0.5f, 1.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f},
		//down
		{0.5f, 0.5f, 0.0f,		0.0f, 0.0f, -1.0f,	0.0f, 0.0f},
		{0.5f, -0.5f, 0.0f,		0.0f, 0.0f, -1.0f,	0.0f, 1.0f},
		{-0.5f, 0.5f, 0.0f,		0.0f, 0.0f, -1.0f,	1.0f, 0.0f},
		{-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, -1.0f,	1.0f, 1.0f},
		//left
		{-0.5f, -0.5f, 1.0f,	0.0f, -1.0f, 0.0f,	0.0f, 0.0f},
		{0.5f, -0.5f, 1.0f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f},
		{0.5f, -0.5f, 0.0f,		0.0f, -1.0f, 0.0f,	1.0f, 1.0f},
		{-0.5f, -0.5f, 0.0f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f},
		//right
		{-0.5f, 0.5f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f},
		{0.5f, 0.5f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f},
		{0.5f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f},
		{-0.5f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f},
		//front
		{-0.5f, 0.5f, 1.0f,		-1.0f, 0.0f, 0.0f,	0.0f, 0.0f},
		{-0.5f, -0.5f, 1.0f,	-1.0f, 0.0f, 0.0f,	1.0f, 0.0f},
		{-0.5f, -0.5f, 0.0f,	-1.0f, 0.0f, 0.0f,	1.0f, 1.0f},
		{-0.5f, 0.5f, 0.0f,		-1.0f, 0.0f, 0.0f,	0.0f, 1.0f},
		//back
		{0.5f, 0.5f, 1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f},
		{0.5f, -0.5f, 1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f},
		{0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f},
		{0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f},
	};

	long ind[] = 
	{
		//up
		0,1,2,
		2,1,3,
		//down	+4
		5,4,7,
		7,4,6,
		//left	+8
		8,9,10,
		8,10,11,
		//right	+12
		14,13,12,
		15,14,12,
		//front	+16
		16,17,19,
		17,18,19,
		//back	+20
		23,21,20,
		23,22,21
	};

	MeshData * md = new MeshData();

	md->CreateBuffers(false, ver, sizeof(DefaultVertex) * 24, ind, sizeof(long) * 36, pDevice);

	pDefaultMesh = MeshDataPtr(md);
	pDefaultMesh->indicesCount = 36;
	pDefaultMesh->vertexSize = sizeof(DefaultVertex);
	pDefaultMesh->verticesCount = 24;
	pDefaultMesh->vertexSemantic = &defaultVertexSemantic;
	pDefaultMesh->vertexDeclaration = ShiftEngine::GetContextManager()->GetVertexDeclaration(defaultVertexSemantic);
	return pDefaultMesh;
}

ShiftEngine::MeshDataPtr ShiftEngine::Utilities::createPlane()
{
	ID3D10Device * pDevice = ShiftEngine::GetContextManager()->GetDevicePointer();

	static MeshDataPtr pDefaultMesh = nullptr;
	if(pDefaultMesh != nullptr)
		return pDefaultMesh;
	
	DefaultVertex ver[] = 
	{
		//up
		{0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f},
		{0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f},
		{-0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f},
		{-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f},
	};

	long ind[] = 
	{
		//up
		0,1,2,
		2,1,3,
	};

	MeshData * md = new MeshData();

	md->CreateBuffers(false, ver, sizeof(DefaultVertex) * 4, ind, sizeof(long) * 6, pDevice);

	pDefaultMesh = MeshDataPtr(md);
	pDefaultMesh->indicesCount = 6;
	pDefaultMesh->vertexSize = sizeof(DefaultVertex);
	pDefaultMesh->verticesCount = 4;
	pDefaultMesh->vertexSemantic = &defaultVertexSemantic;
	pDefaultMesh->vertexDeclaration = ShiftEngine::GetContextManager()->GetVertexDeclaration(defaultVertexSemantic);
	return pDefaultMesh;
}

ShiftEngine::MeshDataPtr ShiftEngine::Utilities::createSphere()
{
	ID3D10Device * pDevice = ShiftEngine::GetContextManager()->GetDevicePointer();

	static MeshDataPtr pDefaultMesh = nullptr;
	if(pDefaultMesh != nullptr)
		return pDefaultMesh;

	const int subdivisions = 36;
	const float radius = 1.0f;

	DefaultVertex * vertices = new DefaultVertex[subdivisions * subdivisions * 6];
	memset(vertices, 0, sizeof(DefaultVertex) * subdivisions * subdivisions * 6);

	const float step = M_PI / subdivisions;

	for(int i = 0; i < subdivisions; i++) //horizontal step (phi)
	{
		for(int j = 0; j < subdivisions; j++) //vertical step (theta)
		{
			float sinPhi[] =	{sin(2.0f * step * i), sin(2.0f * step * (i+1))};
			float cosPhi[] =	{cos(2.0f * step * i), cos(2.0f * step * (i+1))};
			float sinTheta[] =	{sin(step * j), sin(step * (j+1))};
			float cosTheta[] =	{cos(step * j), cos(step * (j+1))};

			//create 4 vertices patch
			Vector3F patch[4];
			patch[0] = Vector3F(sinTheta[1] * cosPhi[0], sinTheta[1] * sinPhi[0], cosTheta[1]);
			patch[1] = Vector3F(sinTheta[1] * cosPhi[1], sinTheta[1] * sinPhi[1], cosTheta[1]);
			patch[2] = Vector3F(sinTheta[0] * cosPhi[1], sinTheta[0] * sinPhi[1], cosTheta[0]);
			patch[3] = Vector3F(sinTheta[0] * cosPhi[0], sinTheta[0] * sinPhi[0], cosTheta[0]);

			Vector2F texcoords[4];
			texcoords[0] = Vector2F((float)i / subdivisions,		(float)(j+1) / subdivisions);
			texcoords[1] = Vector2F((float)(i+1) / subdivisions,	(float)(j+1) / subdivisions);
			texcoords[2] = Vector2F((float)(i+1) / subdivisions,	(float)j / subdivisions);
			texcoords[3] = Vector2F((float)i / subdivisions,		(float)j / subdivisions);

			memcpy(vertices[i * subdivisions * 6 + j * 6].Pos, patch[0].ptr(), sizeof(Vector3F));
			memcpy(vertices[i * subdivisions * 6 + j * 6].Normal, patch[0].ptr(), sizeof(Vector3F));
			memcpy(vertices[i * subdivisions * 6 + j * 6].Texcoord, texcoords[0].ptr(), sizeof(Vector2F));

			memcpy(vertices[i * subdivisions * 6 + j * 6 + 2].Pos, patch[1].ptr(), sizeof(Vector3F));
			memcpy(vertices[i * subdivisions * 6 + j * 6 + 2].Normal, patch[1].ptr(), sizeof(Vector3F));
			memcpy(vertices[i * subdivisions * 6 + j * 6 + 2].Texcoord, texcoords[1].ptr(), sizeof(Vector2F));

			memcpy(vertices[i * subdivisions * 6 + j * 6 + 1].Pos, patch[2].ptr(), sizeof(Vector3F));
			memcpy(vertices[i * subdivisions * 6 + j * 6 + 1].Normal, patch[2].ptr(), sizeof(Vector3F));
			memcpy(vertices[i * subdivisions * 6 + j * 6 + 1].Texcoord, texcoords[2].ptr(), sizeof(Vector2F));

			memcpy(vertices[i * subdivisions * 6 + j * 6 + 3].Pos, patch[0].ptr(), sizeof(Vector3F));
			memcpy(vertices[i * subdivisions * 6 + j * 6 + 3].Normal, patch[0].ptr(), sizeof(Vector3F));
			memcpy(vertices[i * subdivisions * 6 + j * 6 + 3].Texcoord, texcoords[0].ptr(), sizeof(Vector2F));

			memcpy(vertices[i * subdivisions * 6 + j * 6 + 5].Pos, patch[2].ptr(), sizeof(Vector3F));
			memcpy(vertices[i * subdivisions * 6 + j * 6 + 5].Normal, patch[2].ptr(), sizeof(Vector3F));
			memcpy(vertices[i * subdivisions * 6 + j * 6 + 5].Texcoord, texcoords[2].ptr(), sizeof(Vector2F));

			memcpy(vertices[i * subdivisions * 6 + j * 6 + 4].Pos, patch[3].ptr(), sizeof(Vector3F));
			memcpy(vertices[i * subdivisions * 6 + j * 6 + 4].Normal, patch[3].ptr(), sizeof(Vector3F));
			memcpy(vertices[i * subdivisions * 6 + j * 6 + 4].Texcoord, texcoords[3].ptr(), sizeof(Vector2F));
		}
	}

	MeshData * md = new MeshData();

	md->CreateBuffers(false, vertices, sizeof(DefaultVertex) * subdivisions * subdivisions * 6, nullptr, 0, pDevice);

	pDefaultMesh = MeshDataPtr(md);
	pDefaultMesh->indicesCount = 0;
	pDefaultMesh->vertexSize = sizeof(DefaultVertex);
	pDefaultMesh->verticesCount = subdivisions * subdivisions * 6;
	pDefaultMesh->vertexSemantic = &defaultVertexSemantic;
	pDefaultMesh->vertexDeclaration = ShiftEngine::GetContextManager()->GetVertexDeclaration(defaultVertexSemantic);
	return pDefaultMesh;
}
