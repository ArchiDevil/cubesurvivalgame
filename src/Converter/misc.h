#pragma once

#define LIM_HEADER_VERSION 3

#include <MathLib/math.h>

#include <cstdint>

struct MeshLIMHeader
{
    const uint32_t version = LIM_HEADER_VERSION;
    bool hasNormals = false;
    bool hasTexCoords = false;
    bool hasColors = false;
    uint32_t verticesCount = 0;
    uint32_t indicesCount = 0;
};

struct Vertex
{
    Vertex(const MathLib::Vector3F &Pos = {},
           const MathLib::Vector3F &Normal = {},
           const MathLib::Vector2F &TexCoord = {},
           const MathLib::Vector3F &Color = {})
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
