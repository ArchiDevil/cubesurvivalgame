#include "cChunksStorage.h"

#include <GraphicsEngine/ShiftEngine.h>
#include <cassert>

using namespace ShiftEngine;
using ShiftEngine::Material;

cChunksStorage::cChunksStorage()
    : CenterChunkX(0)
    , CenterChunkY(0)
    , Chunks(nullptr)
    , ChunksPerSide(3)
{
}

cChunksStorage::~cChunksStorage()
{
}

void cChunksStorage::Initialize(int ChunksPerSide, int CenterChunkX, int CenterChunkY, unsigned int chunkWidth)
{
    this->ChunksPerSide = ChunksPerSide;
    this->CenterChunkX = CenterChunkX;
    this->CenterChunkY = CenterChunkY;

    IContextManager * pCtxMgr = GetContextManager();

    landSemantics.addSemantic(ET_FLOAT, 3, ES_Position);
    landSemantics.addSemantic(ET_FLOAT, 3, ES_Normal);
    landSemantics.addSemantic(ET_FLOAT, 1, ES_Color);
    auto landIL = pCtxMgr->GetVertexDeclaration(landSemantics);
    MaterialPtr worldChunkMtl = pCtxMgr->LoadMaterial(L"worldChunk.mtl", L"worldChunk");

    waterSemantics.addSemantic(ET_FLOAT, 3, ES_Position);
    waterSemantics.addSemantic(ET_FLOAT, 3, ES_Normal);
    waterSemantics.addSemantic(ET_FLOAT, 3, ES_Color);
    auto waterIL = pCtxMgr->GetVertexDeclaration(waterSemantics);
    MaterialPtr waterChunkMtl = pCtxMgr->LoadMaterial(L"waterChunk.mtl", L"waterChunk");

    Chunks.reset(new WorldChunk[ChunksPerSide * ChunksPerSide]);
    for (int i = 0; i < ChunksPerSide * ChunksPerSide; i++)
    {
        Chunks[i].Initialize(
            GetSceneGraph()->AddMeshNode(IMeshDataPtr(nullptr), worldChunkMtl.get()),
            GetSceneGraph()->AddMeshNode(IMeshDataPtr(nullptr), waterChunkMtl.get()),
            chunkWidth);
    }

    int LeftX, UpY, RightX, DownY;

    LeftX = CenterChunkX - (ChunksPerSide - 1) / 2;
    RightX = CenterChunkX + (ChunksPerSide - 1) / 2;

    UpY = CenterChunkY - (ChunksPerSide - 1) / 2;
    DownY = CenterChunkY + (ChunksPerSide - 1) / 2;

    for (int ChunkX = LeftX; ChunkX <= RightX; ChunkX++)
    {
        for (int ChunkY = UpY; ChunkY <= DownY; ChunkY++)
        {
            GetChunkPtr(ChunkX, ChunkY)->SetWorldX(ChunkX);
            GetChunkPtr(ChunkX, ChunkY)->SetWorldY(ChunkY);
        }
    }
}

int cChunksStorage::GetChunksPerSide() const
{
    return ChunksPerSide;
}

int cChunksStorage::GetCentralX() const
{
    return CenterChunkX;
}

int cChunksStorage::GetCentralY() const
{
    return CenterChunkY;
}

void cChunksStorage::SetCenterX(int x)
{
    CenterChunkX = x;
}

void cChunksStorage::SetCenterY(int y)
{
    CenterChunkY = y;
}

int cChunksStorage::GetChunkNumPointer(int WorldX, int WorldY) const
{
    int wx = WorldX % ChunksPerSide;
    if (wx < 0)
        wx += ChunksPerSide;

    int wy = WorldY % ChunksPerSide;
    if (wy < 0)
        wy += ChunksPerSide;

    return wx * ChunksPerSide + wy;
}

WorldChunk * cChunksStorage::GetChunkPtr(int WorldX, int WorldY)
{
#ifdef DEBUG
    int ss2 = (ChunksPerSide + 1) / 2;
    assert(abs(WorldX - CenterChunkX) <= ss2);
    assert(abs(WorldY - CenterChunkY) <= ss2);
#endif
    return &Chunks[GetChunkNumPointer(WorldX, WorldY)];
}

const WorldChunk * cChunksStorage::GetChunkPtr(int WorldX, int WorldY) const
{
#ifdef DEBUG
    int ss2 = (ChunksPerSide + 1) / 2;
    assert(abs(WorldX - CenterChunkX) <= ss2);
    assert(abs(WorldY - CenterChunkY) <= ss2);
#endif
    return &Chunks[GetChunkNumPointer(WorldX, WorldY)];
}

bool cChunksStorage::IsBorder(int WorldX, int WorldY) const
{
    if (abs(WorldX - CenterChunkX) == (ChunksPerSide - 1) / 2)
        return true;
    if (abs(WorldY - CenterChunkY) == (ChunksPerSide - 1) / 2)
        return true;

    return false;
}

bool cChunksStorage::IsExist(int WorldX, int WorldY) const
{
    if (abs(WorldX - CenterChunkX) > (ChunksPerSide - 1) / 2)
        return false;
    if (abs(WorldY - CenterChunkY) > (ChunksPerSide - 1) / 2)
        return false;

    return true;
}

bool cChunksStorage::HaveRightNeighbors(int WorldX, int WorldY, ChunkStatus minimalStatus) const
{
    if (GetChunkPtr(WorldX + 1, WorldY)->GetStatus() < minimalStatus) return false;
    if (GetChunkPtr(WorldX - 1, WorldY)->GetStatus() < minimalStatus) return false;
    if (GetChunkPtr(WorldX, WorldY + 1)->GetStatus() < minimalStatus) return false;
    if (GetChunkPtr(WorldX, WorldY - 1)->GetStatus() < minimalStatus) return false;

    if (GetChunkPtr(WorldX + 1, WorldY + 1)->GetStatus() < minimalStatus) return false;
    if (GetChunkPtr(WorldX + 1, WorldY - 1)->GetStatus() < minimalStatus) return false;
    if (GetChunkPtr(WorldX - 1, WorldY + 1)->GetStatus() < minimalStatus) return false;
    if (GetChunkPtr(WorldX - 1, WorldY - 1)->GetStatus() < minimalStatus) return false;

    return true;
}

WorldChunk * cChunksStorage::GetChunksArray()
{
    return Chunks.get();
}

const WorldChunk * cChunksStorage::GetChunksArray() const
{
    return Chunks.get();
}

void cChunksStorage::Unload()
{
    for (int i = 0; i < ChunksPerSide * ChunksPerSide; i++)
    {
        Chunks[i].SetStatus(CS_EMPTY);
        Chunks[i].Hide();
    }
}
