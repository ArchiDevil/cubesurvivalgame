#pragma once

#include <cstdlib>
#include <deque>
#include <vector>
#include <mutex>
#include <string>
#include <memory>

#include <Utilities/ut.h>
#include <MathLib/math/ray.h>
#include <MathLib/math/vector3.h>

#define NOTLOADED 0
#define LOADED 1
#define ALREADYLOADED 2

#define MAX_SUN_LIGHT_INDEX 10

class WorldStorage;
class cChunksStorage;
class WorldGenerator;
class cChunkStreamer;
class WorldTesselator;

enum Action
{
	Load,
	Tesselate
};

struct LoadQuery
{
	LoadQuery(int _x, int _y, Action _action) : x(_x), y(_y), action(_action) {};
	int x, y;
	Action action;
};

class cWorld
{
public:
	cWorld();
	~cWorld();

	void			 Initialize(unsigned int ChunksPerSide, int CentralChunkX, int CentralChunkY, const std::string & worldName);
	void			 Unload();
	void			 GenerateChunk(int x, int y);
	void			 ProcessLoading();
	bool			 SelectColumnByRay(const MathLib::Ray & unprojectedRay, MathLib::Vector3F & out) const;
					 
	//Getters		 
	WorldStorage *	 GetDataStorage();
	cChunksStorage * GetChunksStorage();
	bool			 HaveSolidsNear(int x, int y, int z);
					 
	void			 SetWorldName(const std::string & worldName);
	std::string		 GetWorldName() const;
					 
	//Loaders		 
	void			 ShiftChunkX(int ShiftingValue);
	void			 ShiftChunkY(int ShiftingValue);
					 
private:			 
	void			 FillQueue(int centralX, int centralY, int cps);
	void			 UpdateChunk(int WorldX, int WorldY);
	void			 RemoveRepeatingElems();
					 
	//chunk handlers 
	int				 ProcessChunk(int WorldX, int WorldY, Action action);
	void			 ClearChunkData(int WorldX, int WorldY);
	void			 ComputeUpsideLight(int WorldX, int WorldY);
	void			 ComputeScatteringLight(int WorldX, int WorldY);
					 
	void			 LightProp(int PTR, unsigned char value);
					 
	float			 GetInterpolatedHeight(int x, int y);

	std::unique_ptr<WorldStorage>		DataStorage;
	std::unique_ptr<cChunksStorage>		ChunksStorage;
	std::unique_ptr<WorldGenerator>	    Generator;
	std::unique_ptr<cChunkStreamer>		Streamer;
	std::unique_ptr<WorldTesselator>	Tesselator;

	std::mutex							critSect;
	std::mutex							updatingSection;

	std::deque<LoadQuery>				loadingQueue;
	std::string 						worldName;
};
