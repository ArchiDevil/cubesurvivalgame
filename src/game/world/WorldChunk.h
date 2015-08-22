#pragma once

#include <GraphicsEngine/SceneGraph/MeshNode.h>
#include <MathLib/math.h>

enum ChunkStatus
{
    //chunk is empty
    CS_EMPTY,

    //chunk filled with data
    CS_FILLED,

    //chunk tesselated and can be drawn
    CS_TESSELATED
};

struct WaterVertex
{
    WaterVertex(MathLib::Vector3F _pos, MathLib::Vector3F _normal)
        : pos(_pos), normal(_normal)
    {
    }

    MathLib::Vector3F pos;
    MathLib::Vector3F normal;
};

struct texPos
{
    texPos()
        : animV(0.0f)
    {
    }

    texPos(MathLib::Vector3F p, MathLib::Vector2F t, float av)
        : pos(p)
        , tc(t)
        , animV(av)
    {
    }

    MathLib::Vector3F pos;
    MathLib::Vector2F tc;
    float animV;
};

class WorldChunk
{
public:
    WorldChunk();

    void Initialize(ShiftEngine::MeshNode * _landNode, ShiftEngine::MeshNode * _waterNode, unsigned int chunkWidth);

    int GetWorldX() const;
    int GetWorldY() const;
    void SetWorldX(int _x);
    void SetWorldY(int _y);

    ChunkStatus GetStatus() const;
    void SetStatus(ChunkStatus val);

    void Show();
    void Hide();
    bool CanBeDrawn() const;

    ShiftEngine::MeshNode * GetLandNode();
    ShiftEngine::MeshNode * GetWaterNode();

private:
    WorldChunk(const WorldChunk & ref);

    int WorldX, WorldY;             //world position
    unsigned int chunkWidth;
    ChunkStatus Status;

    ShiftEngine::MeshNode * landNode;   //ONLY LANDSCAPE FOR NOW (it would be updated in future to batch)!
    ShiftEngine::MeshNode * waterNode;
};
