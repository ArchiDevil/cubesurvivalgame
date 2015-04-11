#include "WorldChunk.h"

#include <assert.h>

WorldChunk::WorldChunk()
    : WorldX(0)
    , WorldY(0)
    , Status(CS_EMPTY)
    , landNode(nullptr)
    , waterNode(nullptr)
    , chunkWidth(16)
{
}

void WorldChunk::Initialize(ShiftEngine::MeshNode * _landNode, ShiftEngine::MeshNode * _waterNode, unsigned int _chunkWidth)
{
    this->landNode = _landNode;
    this->waterNode = _waterNode;
    this->chunkWidth = _chunkWidth;
}

int WorldChunk::GetWorldX() const
{
    return WorldX;
}

int WorldChunk::GetWorldY() const
{
    return WorldY;
}

void WorldChunk::SetWorldX(int _x)
{
    WorldX = _x;
    landNode->SetPosition(Vector3F((float)WorldX * (float)chunkWidth, (float)WorldY * (float)chunkWidth, 0.0f));
    waterNode->SetPosition(Vector3F((float)WorldX * (float)chunkWidth, (float)WorldY * (float)chunkWidth, 0.0f));
}

void WorldChunk::SetWorldY(int _y)
{
    WorldY = _y;
    landNode->SetPosition(Vector3F((float)WorldX * (float)chunkWidth, (float)WorldY * (float)chunkWidth, 0.0f));
    waterNode->SetPosition(Vector3F((float)WorldX * (float)chunkWidth, (float)WorldY * (float)chunkWidth, 0.0f));
}

ChunkStatus WorldChunk::GetStatus() const
{
    return Status;
}

void WorldChunk::SetStatus(ChunkStatus val)
{
    Status = val;
}

void WorldChunk::Show()
{
    if (!landNode->GetDataPtr() || landNode->GetDataPtr()->GetVerticesCount() != 0)
        landNode->SetVisibility(true);

    if (!waterNode->GetDataPtr() || waterNode->GetDataPtr()->GetVerticesCount() != 0)
        waterNode->SetVisibility(true);
}

void WorldChunk::Hide()
{
    landNode->SetVisibility(false);
    waterNode->SetVisibility(false);
}

bool WorldChunk::CanBeDrawn() const
{
    return landNode->IsVisible();
}

ShiftEngine::MeshNode * WorldChunk::GetLandNode()
{
    return landNode;
}

ShiftEngine::MeshNode * WorldChunk::GetWaterNode()
{
    return waterNode;
}
