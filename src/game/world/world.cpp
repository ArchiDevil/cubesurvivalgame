#include "world.h"

#include "worldGenerator/WorldGenerator.h"
#include "worldStorage.h"
#include "cChunksStorage.h"
#include "cChunkStreamer.h"
#include "worldTesselator.h"

#include "../Environment/EnvironmentManager.h"
#include "../Game.h"
#include "../Entities/GameObjectsManager.h"

#include <sstream>

#define waterLevel 70

using MathLib::Vector2D;

cWorld::cWorld()
    : DataStorage(nullptr)
    , ChunksStorage(nullptr)
    , Generator(nullptr)
    , Streamer(nullptr)
    , Tesselator(nullptr)
{
}

cWorld::~cWorld()
{
    int size = ChunksStorage->GetChunksPerSide();
    size *= size;
    auto chunks = ChunksStorage->GetChunksArray();

    for (int i = 0; i < size; i++)
        if (chunks[i].GetStatus() > CS_EMPTY)
            Streamer->Save(chunks[i].GetWorldX(), chunks[i].GetWorldY());
}

void cWorld::Initialize(unsigned int ChunksPerSide, int CentralChunkX, int CentralChunkY, const std::string & worldName)
{
    if (ChunksPerSide % 2 == 0)
        ChunksPerSide++;        //необходимо для нечетного числа в чанках

    Generator.reset(new WorldGenerator);
    ChunksStorage.reset(new cChunksStorage);
    DataStorage.reset(new WorldStorage(ChunksPerSide * DataStorage->GetChunkWidth()));
    Streamer.reset(new cChunkStreamer(DataStorage.get()));
    Tesselator.reset(new WorldTesselator(DataStorage.get()));
    Tesselator->SetCriticalSection(&critSect);

    SetWorldName(worldName);

    ChunksStorage->Initialize(ChunksPerSide, CentralChunkX, CentralChunkY, DataStorage->GetChunkWidth());
    FillQueue(CentralChunkX, CentralChunkY, ChunksPerSide);

    std::ostringstream str;
    str << "saves/worlds/" << worldName << "/" << worldName << ".lihm";
    Generator->LoadHeightMap(str.str());
    Generator->GetNoise()->SetFrequency(0.001);
    Generator->GetNoise()->SetOctaves(8);
}

void cWorld::Unload()
{
    this->loadingQueue.clear();
}

void cWorld::ClearChunkData(int WorldX, int WorldY)
{
    unsigned int ChunkW = DataStorage->GetChunkWidth();

    for (unsigned int i = 0; i < ChunkW; ++i)
        for (unsigned int j = 0; j < ChunkW; ++j)
            for (unsigned int k = 0; k < DataStorage->GetColumnsCount(); ++k)
                DataStorage->SetColumn(WorldX * ChunkW + i, WorldY * ChunkW + j, k, BlockColumn(BT_Empty, 0));
}

void cWorld::GenerateChunk(int WorldX, int WorldY)
{
    GameObjectsManager * pEntityMgr = LostIsland::GetGamePtr()->entityMgr.get();

    unsigned int ChunkWidth = DataStorage->GetChunkWidth();

    int FromX = WorldX * ChunkWidth;
    int ToX = WorldX * ChunkWidth + ChunkWidth;

    int FromY = WorldY * ChunkWidth;
    int ToY = WorldY * ChunkWidth + ChunkWidth;

    cNoise * pNoise = Generator->GetNoise();

    for (int i = FromX; i < ToX; i++)
    {
        for (int j = FromY; j < ToY; j++)
        {
            pNoise->SetFrequency(0.007);
            pNoise->SetOctaves(5);

            double noise = pNoise->SimplexNoise(i, j) * 6.0f;
            double distance = MathLib::distance(Vector2D(), Vector2D((double)i, (double)j)) / 400.0;
            int result = (int)((noise + 90.0) * (1.0 - distance));
            unsigned char kMax = result < 3 ? 3 : (unsigned char)result; //+ GetInterpolatedHeight(i, j);

            DataStorage->SetColumn(i, j, 0, BlockColumn(BlockTypes::BT_Stone, kMax));
            if (kMax < waterLevel)
            {
                // water
                DataStorage->SetColumn(i, j, 1, BlockColumn(BlockTypes::BT_Water, waterLevel - kMax + 1));
            }
            else if (kMax >= waterLevel && kMax <= waterLevel + 3)
            {
                // sand
                DataStorage->SetColumn(i, j, 0, BlockColumn(BlockTypes::BT_Stone, waterLevel));
                DataStorage->SetColumn(i, j, 1, BlockColumn(BlockTypes::BT_Sand, kMax - waterLevel + 2));

                pNoise->SetFrequency(0.5);
                pNoise->SetOctaves(4);

                Vector3F position = { (float)i, (float)j, 0.0f };
                noise = pNoise->SimplexNoise(i, j);
                if (noise > 1.1)
                {
                    pEntityMgr->CreateEntity(position, "smooth_stone");
                }
            }
            else
            {
                // grass
                DataStorage->SetColumn(i, j, 1, BlockColumn(BlockTypes::BT_Dirt, 1));
                DataStorage->SetColumn(i, j, 2, BlockColumn(BlockTypes::BT_Grass, 1));

                Vector3F position = { (float)i, (float)j, 0.0f };
                pNoise->SetFrequency(0.4);
                pNoise->SetOctaves(6);

                noise = pNoise->SimplexNoise(i, j);
                if (noise > 1.4)
                {
                    pEntityMgr->CreateEntity(position, "tree1");
                }
            }
        }
    }
}

void cWorld::ShiftChunkX(int /*ShiftingValue*/)
{
    //updatingSection.lock();

    //int ccy = ChunksStorage->GetCenterY();
    //int width = (ChunksStorage->GetChunksPerSide() - 1) / 2;
    //int xUnload = 0;
    //int xLoad = 0;

    //if (ShiftingValue > 0)
    //{
    //	xUnload = ChunksStorage->GetCenterX() - width;
    //	xLoad = ChunksStorage->GetCenterX() + width + 1;
    //}
    //else
    //{
    //	xUnload = ChunksStorage->GetCenterX() + width;
    //	xLoad = ChunksStorage->GetCenterX() - width - 1;
    //}

    //for (int i = ccy - width; i <= ccy + width; i++)
    //{
    //	WorldChunk * ChunkPtr = ChunksStorage->GetChunkPtr(xUnload, i);

    //	if (ChunkPtr->GetStatus() > CS_EMPTY)
    //		Streamer->Save(ChunkPtr->GetWorldX(), ChunkPtr->GetWorldY());

    //	ChunkPtr->SetWorldX(xLoad);
    //	ChunkPtr->SetStatus(CS_EMPTY);
    //	ChunkPtr->Hide();

    //	loadingQueue.push_back(LoadQuery(xLoad, i, Load));
    //	loadingQueue.push_back(LoadQuery(xLoad, i, Tesselate));
    //}

    //ChunksStorage->SetCenterX(ChunksStorage->GetCenterX() + ShiftingValue);

    //updatingSection.unlock();
}

void cWorld::ShiftChunkY(int /*ShiftingValue*/)
{
    //updatingSection.lock();

    //int ccx = ChunksStorage->GetCenterX();
    //int width = (ChunksStorage->GetChunksPerSide() - 1) / 2;
    //int yUnload = 0;
    //int yLoad = 0;

    //if (ShiftingValue > 0)
    //{
    //	yUnload = ChunksStorage->GetCenterY() - width;
    //	yLoad = ChunksStorage->GetCenterY() + width + 1;
    //}
    //else
    //{
    //	yUnload = ChunksStorage->GetCenterY() + width;
    //	yLoad = ChunksStorage->GetCenterY() - width - 1;
    //}

    //for (int i = ccx - width; i <= ccx + width; i++)
    //{
    //	WorldChunk * ChunkPtr = ChunksStorage->GetChunkPtr(i, yUnload);

    //	if (ChunkPtr->GetStatus() > CS_EMPTY)
    //		Streamer->Save(ChunkPtr->GetWorldX(), ChunkPtr->GetWorldY());

    //	ChunkPtr->SetWorldY(yLoad);
    //	ChunkPtr->SetStatus(CS_EMPTY);
    //	ChunkPtr->Hide();
    //	loadingQueue.push_back(LoadQuery(i, yLoad, Load));
    //	loadingQueue.push_back(LoadQuery(i, yLoad, Tesselate));
    //}

    //ChunksStorage->SetCenterY(ChunksStorage->GetCenterY() + ShiftingValue);

    //updatingSection.unlock();
}

void cWorld::ProcessLoading()
{
    updatingSection.lock();

    for (auto iter = loadingQueue.begin(); iter != loadingQueue.end(); ++iter)
    {
        if (!ChunksStorage->IsExist((*iter).x, (*iter).y))
        {
            iter = loadingQueue.erase(iter);
            continue;
        }

        int status = ProcessChunk((*iter).x, (*iter).y, (*iter).action);

        if (status == LS_Loaded)
        {
            iter = loadingQueue.erase(iter);
            break;
        }
        else if (status == LS_AlreadyLoaded)
        {
            iter = loadingQueue.erase(iter);
        }
    }

    updatingSection.unlock();
}

cChunksStorage * cWorld::GetChunksStorage()
{
    return ChunksStorage.get();
}

WorldStorage * cWorld::GetDataStorage()
{
    return DataStorage.get();
}

void cWorld::UpdateChunk(int WorldX, int WorldY)
{
    ChunksStorage->GetChunkPtr(WorldX, WorldY)->SetStatus(CS_FILLED);
    loadingQueue.push_front(LoadQuery(WorldX, WorldY, Tesselate));
}

cWorld::LoadingStatus cWorld::ProcessChunk(int WorldX, int WorldY, Action action)
{
    WorldChunk * ChunkPtr = ChunksStorage->GetChunkPtr(WorldX, WorldY);

    if (action == Load)
    {
        if (ChunkPtr->GetStatus() > CS_EMPTY)
            return LS_AlreadyLoaded;

        ClearChunkData(WorldX, WorldY);

        if (Streamer->Check(WorldX, WorldY))
            Streamer->Load(WorldX, WorldY);
        else
            GenerateChunk(WorldX, WorldY);

        ChunkPtr->SetStatus(CS_FILLED);
        return LS_Loaded;
    }

    if (action == Tesselate)
    {
        if (ChunkPtr->GetStatus() > CS_FILLED)
            return LS_AlreadyLoaded;
        if (ChunksStorage->IsBorder(WorldX, WorldY))
            return LS_NotLoaded;
        if (!ChunksStorage->HaveRightNeighbors(WorldX, WorldY, CS_FILLED))
            return LS_NotLoaded;

        if (Tesselator->TesselateChunk(WorldX, WorldY, ChunkPtr->GetLandNode(), ChunkPtr->GetWaterNode()))
        {
            //Tesselator->BuildWaterMesh(WorldX, WorldY);
            ChunkPtr->SetStatus(CS_TESSELATED);
            ChunkPtr->Show();
            return LS_Loaded;
        }
        else
        {
            return LS_NotLoaded;
        }
    }

    return LS_NotLoaded;
}

float cWorld::GetInterpolatedHeight(int x, int y)
{
    unsigned int ChunkWidth = DataStorage->GetChunkWidth();

    int LeftChunkX = (int)floor((float)(x - (int)ChunkWidth / 2) / (int)ChunkWidth);
    int RightChunkX = LeftChunkX + 1;
    int TopChunkY = (int)floor((float)(y - (int)ChunkWidth / 2) / (int)ChunkWidth);
    int BottomChunkY = TopChunkY + 1;

    int left = LeftChunkX * ChunkWidth + (int)ChunkWidth / 2;
    int top = TopChunkY * ChunkWidth + (int)ChunkWidth / 2;

    float k = (float)(x - left) / (int)ChunkWidth;
    float v1 = MathLib::LinearInterpolation((float)Generator->GetHeight(LeftChunkX, TopChunkY),
                                            (float)Generator->GetHeight(RightChunkX, TopChunkY), k);
    float v2 = MathLib::LinearInterpolation((float)Generator->GetHeight(LeftChunkX, BottomChunkY),
                                            (float)Generator->GetHeight(RightChunkX, BottomChunkY), k);
    k = (float)(y - top) / (int)ChunkWidth;
    float v3 = MathLib::LinearInterpolation(v1, v2, k);

    return v3;
}

void cWorld::FillQueue(int centralX, int centralY, int cps)
{
    int width2 = (cps - 1) / 2;

    loadingQueue.push_back(LoadQuery(centralX, centralY, Load));
    loadingQueue.push_back(LoadQuery(centralX, centralY, Tesselate));

    for (int i = 0; i <= width2; i++)	//for all
    {
        int startX = centralX - i;
        int startY = centralY - i;
        int endX = centralX + i;
        int endY = centralY + i;

        for (int index = startX; index < endX; index++)
        {
            loadingQueue.push_back(LoadQuery(index, startY, Load));
            loadingQueue.push_back(LoadQuery(index, startY, Tesselate));
        }

        for (int index = startY; index < endY; index++)
        {
            loadingQueue.push_back(LoadQuery(endX, index, Load));
            loadingQueue.push_back(LoadQuery(endX, index, Tesselate));
        }

        for (int index = endX; index > startX; index--)
        {
            loadingQueue.push_back(LoadQuery(index, endY, Load));
            loadingQueue.push_back(LoadQuery(index, endY, Tesselate));
        }

        for (int index = endY; index > startY; index--)
        {
            loadingQueue.push_back(LoadQuery(startX, index, Load));
            loadingQueue.push_back(LoadQuery(startX, index, Tesselate));
        }
    }
}

void cWorld::SetWorldName(const std::string & worldName)
{
    this->worldName = worldName;
}

std::string cWorld::GetWorldName() const
{
    return worldName;
}

bool cWorld::SelectColumnByRay(const MathLib::Ray & unprojectedRay, Vector3F & out) const
{
    std::vector<WorldChunk*> foundChunksList;
    unsigned int chunksPerSide = ChunksStorage->GetChunksPerSide();
    for (unsigned int i = 0; i < chunksPerSide * chunksPerSide; ++i)
    {
        WorldChunk* pChunk = ChunksStorage->GetChunksArray() + i;
        auto bbox = pChunk->GetLandNode()->GetBBox();

        if (MathLib::RayBoxIntersect(unprojectedRay, bbox, 0.0f, 1000.0f))
            foundChunksList.push_back(pChunk);
    }

    std::vector<MathLib::AABB> foundBBoxes(foundChunksList.size());
    for (WorldChunk * pChunk : foundChunksList)
    {
        int WorldX = pChunk->GetWorldX();
        int WorldY = pChunk->GetWorldY();
        int ChunkWidth = DataStorage->GetChunkWidth();
        for (int xStart = WorldX * ChunkWidth; xStart < WorldX * ChunkWidth + ChunkWidth; ++xStart)
        {
            for (int yStart = WorldY * ChunkWidth; yStart < WorldY * ChunkWidth + ChunkWidth; ++yStart)
            {
                MathLib::AABB bbox;
                bbox.bMin = Vector3F((float)xStart, (float)yStart, (float)DataStorage->GetFullHeight(xStart, yStart));
                bbox.bMax = Vector3F((float)xStart + 1.0f, (float)yStart + 1.0f, (float)DataStorage->GetFullHeight(xStart, yStart) + 0.1f);
                if (MathLib::RayBoxIntersect(unprojectedRay, bbox, 0.0f, 1000.0f))
                    foundBBoxes.push_back(bbox);
            }
        }
    }

    float minimalDistance = 1000.0f;
    const MathLib::AABB * resultedBBox = nullptr;
    for (const MathLib::AABB & bbox : foundBBoxes)
    {
        float distance = MathLib::distance(bbox.GetCentralPoint(), unprojectedRay.Origin);
        if (distance < minimalDistance)
        {
            minimalDistance = distance;
            resultedBBox = &bbox;
        }
    }

    if (!resultedBBox)
    {
        out = {};
        return false;
    }

    Vector3F topPoints[4] =
    {
        { resultedBBox->bMin.x, resultedBBox->bMin.y, resultedBBox->bMax.z },
        { resultedBBox->bMin.x, resultedBBox->bMax.y, resultedBBox->bMax.z },
        { resultedBBox->bMax.x, resultedBBox->bMin.y, resultedBBox->bMax.z },
        { resultedBBox->bMax.x, resultedBBox->bMax.y, resultedBBox->bMax.z }
    };

    MathLib::Vector3F outPoint = {};
    if (MathLib::LineTriangleIntersectionPoint(topPoints[0], topPoints[1], topPoints[2], unprojectedRay.Origin, unprojectedRay.Origin + unprojectedRay.Direction * 256.0f, outPoint))
    {
        out = outPoint;
        return true;
    }

    if (MathLib::LineTriangleIntersectionPoint(topPoints[1], topPoints[2], topPoints[3], unprojectedRay.Origin, unprojectedRay.Origin + unprojectedRay.Direction * 256.0f, outPoint))
    {
        out = outPoint;
        return true;
    }

    assert(false); // something gone wrong
    return false;
}
