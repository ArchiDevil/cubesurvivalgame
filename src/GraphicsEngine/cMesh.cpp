#include "cMesh.h"

#define SAFE_RELEASE(obj) if (obj) { obj->Release(); obj = nullptr; }

ShiftEngine::MeshData::MeshData( D3D10VDPtr _vertexDeclaration, ID3D10Buffer * _VB /*= nullptr*/, ID3D10Buffer * _IB /*= nullptr*/ )
	: VertexBuffer(_VB), IndexBuffer(_IB), verticesCount(0), 
	indicesCount(0), vertexSize(0), vertexDeclaration(_vertexDeclaration),
	vertexSemantic(nullptr)
{
}

ShiftEngine::MeshData::MeshData( const MeshData & ref )
{
	this->indicesCount = ref.indicesCount;
	this->vertexSize = ref.vertexSize;
	this->verticesCount = ref.verticesCount;
	
	this->IndexBuffer = ref.IndexBuffer;
	if(this->IndexBuffer)
		IndexBuffer->AddRef();

	this->VertexBuffer = ref.VertexBuffer;
	if(this->VertexBuffer)
		VertexBuffer->AddRef();

	this->vertexDeclaration = ref.vertexDeclaration;

	this->vertexSemantic = ref.vertexSemantic;
}

ShiftEngine::MeshData::~MeshData()
{
	Clear();
}

bool ShiftEngine::MeshData::CreateBuffers( bool dynamic, 
							const void * vData, size_t vSize, 
							const void * iData, size_t iSize, 
							ID3D10Device * device )
{
	if(vSize > 0 && !vData)	return false;
	if(iSize > 0 && !iData)	return false;

	D3D10_BUFFER_DESC vBuffDesc;
	ZeroMemory(&vBuffDesc, sizeof(D3D10_BUFFER_DESC));
	D3D10_BUFFER_DESC iBuffDesc;
	ZeroMemory(&iBuffDesc, sizeof(D3D10_BUFFER_DESC));

	vBuffDesc.BindFlags			= D3D10_BIND_VERTEX_BUFFER;
	vBuffDesc.ByteWidth			= vSize;
	vBuffDesc.MiscFlags			= NULL;

	if(dynamic)
	{
		vBuffDesc.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE;
		vBuffDesc.Usage				= D3D10_USAGE_DYNAMIC;
	}
	else
	{
		vBuffDesc.CPUAccessFlags	= NULL;
		vBuffDesc.Usage				= D3D10_USAGE_DEFAULT;
	}

	if(vSize > 0)
	{
		D3D10_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D10_SUBRESOURCE_DATA));
		data.pSysMem = vData;

		if(FAILED(device->CreateBuffer(&vBuffDesc, &data, &VertexBuffer)))
			return false;
	}

	iBuffDesc.BindFlags			= D3D10_BIND_INDEX_BUFFER;
	iBuffDesc.ByteWidth			= iSize;
	iBuffDesc.MiscFlags			= NULL;

	if(dynamic)
	{
		iBuffDesc.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE;
		iBuffDesc.Usage				= D3D10_USAGE_DYNAMIC;
	}
	else
	{
		iBuffDesc.CPUAccessFlags	= NULL;
		iBuffDesc.Usage				= D3D10_USAGE_DEFAULT;
	}

	if(iSize > 0)
	{
		D3D10_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D10_SUBRESOURCE_DATA));
		data.pSysMem = iData;

		if(FAILED(device->CreateBuffer(&iBuffDesc, &data, &IndexBuffer)))
			return false;
	}

	return true;
}

void ShiftEngine::MeshData::Clear()
{
	SAFE_RELEASE(VertexBuffer);
	SAFE_RELEASE(IndexBuffer);
}

int ShiftEngine::MeshData::Draw( ID3D10Device * device )
{
	if(!device)
		return 0;

	if(!vertexSize || !verticesCount)
		return 0;

	if(indicesCount > 0)
	{
		unsigned int stride = vertexSize;
		unsigned int offset = 0;
		device->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
		device->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		device->DrawIndexed(indicesCount, 0, 0);

		return indicesCount / 3;
	}
	else
	{
		unsigned int stride = vertexSize;
		unsigned int offset = 0;
		device->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
		device->Draw(verticesCount, 0);

		return verticesCount / 3;
	}
}