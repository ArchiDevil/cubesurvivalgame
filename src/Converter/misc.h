#pragma once

#include <string>
#include <cstring>

#define LIM_HEADER_VERSION 3

#include <MathLib/math.h>

using MathLib::Vector2F;
using MathLib::Vector3F;

struct MeshLIMHeader
{
	MeshLIMHeader(int version = 0, 
				bool hasNormals = false, 
				bool hasTexcoords = false, 
				unsigned int verticesCount = 0, 
				unsigned int indicesCount = 0) 
                : version(version)
                , hasNormals(hasNormals)
                , hasTexCoords(hasTexcoords)
                , verticesCount(verticesCount)
                , indicesCount(indicesCount)
	{
    }

    unsigned int version;
	bool hasNormals;
	bool hasTexCoords;
    bool hasColors;
	unsigned int verticesCount;
	unsigned int indicesCount;
};

struct Vertex
{
    Vertex(const Vector3F &Pos = {}, const Vector3F &Normal = {}, const Vector2F &TexCoord = {}, const Vector3F &Color = {})
        : Pos(Pos)
        , Normal(Normal)
        , TexCoord(TexCoord)
        , Color(Color)
	{
    }

	Vector3F Pos;
	Vector3F Normal;
	Vector2F TexCoord;
    Vector3F Color;
};

