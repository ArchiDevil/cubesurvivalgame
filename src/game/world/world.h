#pragma once

#include <cstdlib>
#include <deque>
#include <vector>
#include <mutex>
#include <string>
#include <memory>

#include <Utilities/ut.h>
#include <MathLib/math.h>

class WorldStorage;
class cChunksStorage;
class WorldGenerator;
class cChunkStreamer;
class WorldTesselator;

class cWorld
{
    enum Action
    {
        Load,
        Tesselate
    };

    struct LoadQuery
    {
        LoadQuery(int x, int y, Action action)
            : x(x)
            , y(y)
            , action(action)
        {
        }

        int x, y;
        Action action;
    };

    enum LoadingStatus
    {
        LS_NotLoaded,
        LS_Loaded,
        LS_AlreadyLoaded
    };

public:
    ~cWorld();

    void             Initialize(unsigned int ChunksPerSide, int CentralChunkX, int CentralChunkY, const std::string & worldName);
    void             Unload();
    void             GenerateChunk(int x, int y);
    void             ProcessLoading();
    bool             SelectColumnByRay(const MathLib::Ray & unprojectedRay, MathLib::Vector3F & out) const;

    //Getters
    WorldStorage *   GetDataStorage();
    cChunksStorage * GetChunksStorage();
    bool             HaveSolidsNear(int x, int y, int z);

    void             SetWorldName(const std::string & worldName);
    std::string      GetWorldName() const;

    //Loaders
    void             ShiftChunkX(int ShiftingValue);
    void             ShiftChunkY(int ShiftingValue);

private:
    void             FillQueue(int centralX, int centralY, int cps);
    void             UpdateChunk(int WorldX, int WorldY);
    void             RemoveRepeatingElems();

    //chunk handlers
    LoadingStatus    ProcessChunk(int WorldX, int WorldY, Action action);
    void             ClearChunkData(int WorldX, int WorldY);
    void             ComputeUpsideLight(int WorldX, int WorldY);
    void             ComputeScatteringLight(int WorldX, int WorldY);

    void             LightProp(int PTR, unsigned char value);

    float            GetInterpolatedHeight(int x, int y);

    std::unique_ptr<WorldStorage>       DataStorage = nullptr;
    std::unique_ptr<cChunksStorage>     ChunksStorage = nullptr;
    std::unique_ptr<WorldGenerator>     Generator = nullptr;
    std::unique_ptr<cChunkStreamer>     Streamer = nullptr;
    std::unique_ptr<WorldTesselator>    Tesselator = nullptr;

    std::mutex                          critSect;
    std::mutex                          updatingSection;

    std::deque<LoadQuery>               loadingQueue;
    std::string                         worldName = "";
};
