#pragma once

#define LIM_HEADER_VERSION 3

#include <MathLib/math.h>

using MathLib::Vector2F;
using MathLib::Vector3F;

struct MeshLIMHeader
{
    MeshLIMHeader(int version = 0, bool hasNormals = false, bool hasTexcoords = false, bool hasColors = false,
        unsigned int verticesCount = 0, unsigned int indicesCount = 0)
        : version(version)
        , hasNormals(hasNormals)
        , hasTexCoords(hasTexcoords)
        , hasColors(hasColors)
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
    Vertex(const MathLib::Vector3F &Pos = {}, const MathLib::Vector3F &Normal = {}, const MathLib::Vector2F &TexCoord = {}, const MathLib::Vector3F &Color = {})
        : Pos(Pos)
        , Normal(Normal)
        , TexCoord(TexCoord)
        , Color(Color)
    {
    }

    MathLib::Vector3F Pos;
    MathLib::Vector3F Normal;
    MathLib::Vector2F TexCoord;
    MathLib::Vector3F Color;
};

