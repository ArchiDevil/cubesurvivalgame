#pragma once

#include <d3d10.h>
#include <string>
#include <memory>

#include "D3D10VertexDeclaration.h"

#include "VertexTypes.h"
#include "MiscTypes.h"

namespace ShiftEngine
{
	//mesh must contain AABB
	struct MeshData
	{
		MeshData(D3D10VDPtr _vertexDeclaration = nullptr, ID3D10Buffer * _VB = nullptr, ID3D10Buffer * _IB = nullptr);
		MeshData(const MeshData & ref);
		~MeshData();

		bool CreateBuffers(bool dynamic, const void * vData, size_t vSize, 
			const void * iData, size_t iSize, ID3D10Device * device);
		int Draw(ID3D10Device * device);
		void Clear();

		const VertexSemantic * vertexSemantic;
		D3D10VDPtr vertexDeclaration;
		ID3D10Buffer * VertexBuffer;
		ID3D10Buffer * IndexBuffer;
		unsigned int verticesCount;
		unsigned int indicesCount;
		unsigned int vertexSize;
	};

	typedef std::shared_ptr<MeshData> MeshDataPtr;
}
