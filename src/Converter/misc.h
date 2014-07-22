#pragma once

#include <string>
#include <cstring>

#define LIM_HEADER_VERSION 2

#include "../MathLib/math.h"

using MathLib::Vector2F;
using MathLib::Vector3F;

//хидер для файла с моделями
struct MeshLIMHeader
{
	MeshLIMHeader(int ver = 0, 
				bool hn = false, 
				bool htc = false, 
				unsigned int vc = 0, 
				unsigned int ic = 0) 
		: VERSION(ver), hasNormals(hn), hasTexCoords(htc), verticesCount(vc), indicesCount(ic) {};
	unsigned int VERSION;
	bool hasNormals;
	bool hasTexCoords;
	unsigned int verticesCount;
	unsigned int indicesCount;
};

struct Vertex
{
	Vertex(Vector3F _pos = Vector3F(), Vector3F _normal = Vector3F(), Vector2F _tc = Vector2F()) 
		: Pos(_pos), Normal(_normal), TexCoord(_tc) 
	{}

	Vector3F Pos;
	Vector3F Normal;
	Vector2F TexCoord;
};

