#include "WorldChunk.h"

#include <assert.h>

WorldChunk::WorldChunk() 
	: WorldX(0), WorldY(0), Pointer(0), Status(CS_EMPTY),
	landNode(nullptr), waterNode(nullptr),
	chunkHeight(128), chunkWidth(16)
{
}

void WorldChunk::Initialize( ShiftEngine::MeshNode * _landNode, ShiftEngine::MeshNode * _waterNode, unsigned int _chunkWidth, unsigned int _chunkHeight )
{
	this->landNode = _landNode;
	this->waterNode = _waterNode;
	this->chunkHeight = _chunkHeight;
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

void WorldChunk::SetWorldX( int _x )
{
	WorldX = _x;
	landNode->SetPosition(Vector3F(WorldX * (int)chunkWidth, WorldY * (int)chunkWidth, 0.0f));
	waterNode->SetPosition(Vector3F(WorldX * (int)chunkWidth, WorldY * (int)chunkWidth, 0.0f));
}

void WorldChunk::SetWorldY( int _y )
{
	WorldY = _y;
	landNode->SetPosition(Vector3F(WorldX * (int)chunkWidth, WorldY * (int)chunkWidth, 0.0f));
	waterNode->SetPosition(Vector3F(WorldX * (int)chunkWidth, WorldY * (int)chunkWidth, 0.0f));
}

ChunkStatus WorldChunk::GetStatus() const
{
	return Status;
}

void WorldChunk::SetStatus( ChunkStatus val )
{
	Status = val;
}

void WorldChunk::Show()
{
	if(landNode->GetDataPtr()->verticesCount != 0)
		landNode->SetVisibility(true);

	if(waterNode->GetDataPtr()->verticesCount != 0)
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
