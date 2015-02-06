#pragma once

#include <GraphicsEngine/SceneGraph/SceneGraph.h>
#include <MathLib/math.h>

#include "WorldStorage.h"
#include "worldGenerator/cNoise.h"

#include <mutex>

using MathLib::Vector3D;
using MathLib::Vector3I;
using MathLib::Vector3F;

class WorldTesselator
{
    struct PNC
    {
        PNC(const Vector3F & Pos, const Vector3F & Normal, const Vector3F & Color)
            : Pos(Pos)
            , Normal(Normal)
            , Color(Color)
        {
        }
        Vector3F Pos;
        Vector3F Normal;
        Vector3F Color;
    };

public:
    WorldTesselator(WorldStorage * _ws);
    ~WorldTesselator();

    bool TesselateChunk(int ChunkX, int ChunkY, ShiftEngine::MeshNode * landNode, ShiftEngine::MeshNode * waterNode);
    void SetCriticalSection(std::mutex * sect);

private:
    void TesselateSubChunk(std::vector<PNC> &vertices, std::vector<unsigned long> &indices, const Vector3F &blockPos, const Vector3F &color, int chunkX, int chunkY);

    void PushLeftSide(std::vector<PNC> &vertices, std::vector<unsigned long> &indices, const Vector3F &color, const Vector3F &pos, const Vector3F &sizes = { 1.0f, 1.0f, 1.0f })
    {
        unsigned long curIndex = (unsigned long)vertices.size();

        vertices.push_back(PNC(Vector3F(pos.x + 0.0f, pos.y + sizes.y, pos.z + sizes.z), NormXp, color));
        vertices.push_back(PNC(Vector3F(pos.x + 0.0f, pos.y + 0.0f, pos.z + sizes.z), NormXp, color));
        vertices.push_back(PNC(Vector3F(pos.x + 0.0f, pos.y + 0.0f, pos.z + 0.0f), NormXp, color));
        vertices.push_back(PNC(Vector3F(pos.x + 0.0f, pos.y + sizes.y, pos.z + 0.0f), NormXp, color));

        indices.push_back(curIndex + 0);
        indices.push_back(curIndex + 1);
        indices.push_back(curIndex + 2);

        indices.push_back(curIndex + 0);
        indices.push_back(curIndex + 2);
        indices.push_back(curIndex + 3);
    }

    void PushRightSide(std::vector<PNC> &vertices, std::vector<unsigned long> &indices, const Vector3F &color, const Vector3F &pos, const Vector3F &sizes = { 1.0f, 1.0f, 1.0f })
    {
        unsigned long curIndex = vertices.size();

        vertices.push_back(PNC(Vector3F(pos.x + sizes.x, pos.y + 0.0f, pos.z + sizes.z), NormXm, color));
        vertices.push_back(PNC(Vector3F(pos.x + sizes.x, pos.y + sizes.y, pos.z + sizes.z), NormXm, color));
        vertices.push_back(PNC(Vector3F(pos.x + sizes.x, pos.y + sizes.y, pos.z + 0.0f), NormXm, color));
        vertices.push_back(PNC(Vector3F(pos.x + sizes.x, pos.y + 0.0f, pos.z + 0.0f), NormXm, color));

        indices.push_back(curIndex + 0);
        indices.push_back(curIndex + 1);
        indices.push_back(curIndex + 2);

        indices.push_back(curIndex + 0);
        indices.push_back(curIndex + 2);
        indices.push_back(curIndex + 3);
    }

    void PushFrontSide(std::vector<PNC> &vertices, std::vector<unsigned long> &indices, const Vector3F &color, const Vector3F &pos, const Vector3F &sizes = { 1.0f, 1.0f, 1.0f })
    {
        unsigned long curIndex = vertices.size();

        vertices.push_back(PNC(Vector3F(pos.x + 0.0f, pos.y + 0.0f, pos.z + sizes.z), NormYm, color));
        vertices.push_back(PNC(Vector3F(pos.x + sizes.x, pos.y + 0.0f, pos.z + sizes.z), NormYm, color));
        vertices.push_back(PNC(Vector3F(pos.x + sizes.x, pos.y + 0.0f, pos.z + 0.0f), NormYm, color));
        vertices.push_back(PNC(Vector3F(pos.x + 0.0f, pos.y + 0.0f, pos.z + 0.0f), NormYm, color));

        indices.push_back(curIndex + 0);
        indices.push_back(curIndex + 1);
        indices.push_back(curIndex + 2);

        indices.push_back(curIndex + 0);
        indices.push_back(curIndex + 2);
        indices.push_back(curIndex + 3);
    }

    void PushBackSide(std::vector<PNC> &vertices, std::vector<unsigned long> &indices, const Vector3F &color, const Vector3F &pos, const Vector3F &sizes = { 1.0f, 1.0f, 1.0f })
    {
        unsigned long curIndex = vertices.size();

        vertices.push_back(PNC(Vector3F(pos.x + sizes.x, pos.y + sizes.y, pos.z + sizes.z), NormYp, color));
        vertices.push_back(PNC(Vector3F(pos.x + 0.0f, pos.y + sizes.y, pos.z + sizes.z), NormYp, color));
        vertices.push_back(PNC(Vector3F(pos.x + 0.0f, pos.y + sizes.y, pos.z + 0.0f), NormYp, color));
        vertices.push_back(PNC(Vector3F(pos.x + sizes.x, pos.y + sizes.y, pos.z + 0.0f), NormYp, color));

        indices.push_back(curIndex + 0);
        indices.push_back(curIndex + 1);
        indices.push_back(curIndex + 2);

        indices.push_back(curIndex + 0);
        indices.push_back(curIndex + 2);
        indices.push_back(curIndex + 3);
    }

    WorldStorage * ws;
    std::mutex * criticalSection;
    ShiftEngine::VertexSemantic nodeSemantic;
    ShiftEngine::D3D10VDPtr nodeDeclaration;
    cNoise noiseGenerator;

    const Vector3F NormUP = { 0.0f, 0.0f, 1.0f };
    const Vector3F NormXp = { 1.0f, 0.0f, 0.0f };
    const Vector3F NormXm = { -1.0f, 0.0f, 0.0f };
    const Vector3F NormYp = { 0.0f, 1.0f, 0.0f };
    const Vector3F NormYm = { 0.0f, -1.0f, 0.0f };

};
