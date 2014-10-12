#pragma once

#include <GraphicsEngine/SceneGraph/SceneGraph.h>

#include "WorldStorage.h"

#include <mutex>

class WorldTesselator
{
public:
	WorldTesselator(WorldStorage * _ws);
	~WorldTesselator();

	bool TesselateChunk(int ChunkX, int ChunkY, ShiftEngine::MeshNode * node);
	void SetCriticalSection(std::mutex * sect);

private:
	WorldStorage * ws;
	std::mutex * criticalSection;
	ShiftEngine::D3D10VDPtr vd;

};
