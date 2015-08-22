#include "WorldTesselator.h"

#include <GraphicsEngine/ShiftEngine.h>

WorldTesselator::WorldTesselator(WorldStorage * _ws)
    : ws(_ws)
    , criticalSection(nullptr)
{
    nodeSemantic.addSemantic(ShiftEngine::ET_FLOAT, 3, ShiftEngine::ES_Position);
    nodeSemantic.addSemantic(ShiftEngine::ET_FLOAT, 3, ShiftEngine::ES_Normal);
    nodeSemantic.addSemantic(ShiftEngine::ET_FLOAT, 3, ShiftEngine::ES_Color);
    noiseGenerator.SetFrequency(0.01);
}

WorldTesselator::~WorldTesselator()
{
}

bool WorldTesselator::TesselateChunk(int ChunkX, int ChunkY, ShiftEngine::MeshNode * landNode, ShiftEngine::MeshNode * waterNode)
{
    static const MathLib::Vector3F colors[] =
    {
        {0.0f, 2.0f, 2.0f},									//EMPTY COLOR
        {231.0f / 255.0f, 208.0f / 255.0f, 82.0f / 255.0f},	//SAND COLOR
        {107.0f / 255.0f, 95.0f / 255.0f, 28.0f / 255.0f},	//DIRT COLOR
        {40.0f / 255.0f, 161.0f / 255.0f, 40.0f / 255.0f},	//GRASS COLOR
        {99.0f / 255.0f, 99.0f / 255.0f, 99.0f / 255.0f},	//STONE COLOR
    };

    const int chunkWidth = ws->GetChunkWidth();
    int blockXstart = ChunkX * chunkWidth;
    int blockYstart = ChunkY * chunkWidth;

    std::vector<PNC> vertices;
    std::vector<uint32_t> indices;

    MathLib::AABB bbox;
    bbox.bMin.x = 0.0f;
    bbox.bMin.y = 0.0f;
    bbox.bMin.z = 1000.0f;

    bbox.bMax.x = (float)chunkWidth;
    bbox.bMax.y = (float)chunkWidth;

    vertices.reserve(10000);
    indices.reserve(40000);

    for (int i = 0; i < chunkWidth; i++)
    {
        for (int j = 0; j < chunkWidth; j++)
        {
            float shift1 = (float)noiseGenerator.SimplexNoise((double)(i + blockXstart), (double)(j + blockYstart)) * 30.0f / 255.0f;
            MathLib::Vector3F randVector = MathLib::Vector3F(shift1, shift1, shift1);

            unsigned long curIndex = vertices.size();

            int height = ws->GetFullHeight(blockXstart + i, blockYstart + j);
            if ((float)height > bbox.bMax.z)
                bbox.bMax.z = (float)height;
            if ((float)height < bbox.bMin.z)
                bbox.bMin.z = (float)height;

            //UP

            auto blockType = ws->GetBlockType(blockXstart + i, blockYstart + j, (unsigned int)height - 1);
            while (blockType == BT_Water)
            {
                height--;
                blockType = ws->GetBlockType(blockXstart + i, blockYstart + j, (unsigned int)height - 1);
            }
            MathLib::Vector3F color = colors[blockType];
            unsigned int targetedHeight = (unsigned int)height;
            BlockTypes blocks[3][3];
            for (int w = -1; w <= 1; ++w)
                for (int h = -1; h <= 1; ++h)
                    blocks[w + 1][h + 1] = ws->GetBlockType(blockXstart + i + w, blockYstart + j + h, targetedHeight);

            float colorModifier = 1.0f;
            if (blocks[0][1] != BT_Empty) colorModifier -= 0.25f;
            if (blocks[0][2] != BT_Empty) colorModifier -= 0.25f;
            if (blocks[1][2] != BT_Empty) colorModifier -= 0.25f;
            vertices.push_back(PNC(MathLib::Vector3F(i + 0.0f, j + 1.0f, (float)height), NormUP, (color + randVector) * colorModifier));

            colorModifier = 1.0f;
            if (blocks[2][2] != BT_Empty) colorModifier -= 0.25f;
            if (blocks[2][1] != BT_Empty) colorModifier -= 0.25f;
            if (blocks[1][2] != BT_Empty) colorModifier -= 0.25f;
            vertices.push_back(PNC(MathLib::Vector3F(i + 1.0f, j + 1.0f, (float)height), NormUP, (color + randVector) * colorModifier));

            colorModifier = 1.0f;
            if (blocks[1][0] != BT_Empty) colorModifier -= 0.25f;
            if (blocks[2][0] != BT_Empty) colorModifier -= 0.25f;
            if (blocks[2][1] != BT_Empty) colorModifier -= 0.25f;
            vertices.push_back(PNC(MathLib::Vector3F(i + 1.0f, j + 0.0f, (float)height), NormUP, (color + randVector) * colorModifier));

            colorModifier = 1.0f;
            if (blocks[0][0] != BT_Empty) colorModifier -= 0.25f;
            if (blocks[1][0] != BT_Empty) colorModifier -= 0.25f;
            if (blocks[0][1] != BT_Empty) colorModifier -= 0.25f;
            vertices.push_back(PNC(MathLib::Vector3F(i + 0.0f, j + 0.0f, (float)height), NormUP, (color + randVector) * colorModifier));

            indices.push_back(curIndex + 0);
            indices.push_back(curIndex + 1);
            indices.push_back(curIndex + 2);

            indices.push_back(curIndex + 0);
            indices.push_back(curIndex + 2);
            indices.push_back(curIndex + 3);

            //if (blockType == BT_Grass)
            //    TesselateSubChunk(vertices, indices, { (float)i, (float)j, (float)height }, color + randVector, ChunkX, ChunkY);
            //noiseGenerator.SetFrequency(0.01);

            //LEFT
            //int neighboorHeight = ws->GetFullHeight(blockXstart + i - 1, blockYstart + j);
            int neighboorHeight = height;
            BlockTypes bt = ws->GetBlockType(blockXstart + i - 1, blockYstart + j, neighboorHeight);
            while (bt == BT_Water || bt == BT_Empty)
                bt = ws->GetBlockType(blockXstart + i - 1, blockYstart + j, --neighboorHeight);
            neighboorHeight++;

            if (neighboorHeight < height)
            {
                for (int k = neighboorHeight; k < height; k++)
                {
                    color = colors[ws->GetBlockType(blockXstart + i, blockYstart + j, k)];
                    PushLeftSide(vertices, indices, color, { (float)i, (float)j, (float)k });
                }
            }

            //RIGHT
            neighboorHeight = height;
            bt = ws->GetBlockType(blockXstart + i + 1, blockYstart + j, neighboorHeight);
            while (bt == BT_Water || bt == BT_Empty)
                bt = ws->GetBlockType(blockXstart + i + 1, blockYstart + j, --neighboorHeight);
            neighboorHeight++;

            if (neighboorHeight < height)
            {
                for (int k = neighboorHeight; k < height; k++)
                {
                    color = colors[ws->GetBlockType(blockXstart + i, blockYstart + j, k)];
                    PushRightSide(vertices, indices, color, { (float)i, (float)j, (float)k });
                }
            }

            //FRONT
            neighboorHeight = height;
            bt = ws->GetBlockType(blockXstart + i, blockYstart + j - 1, neighboorHeight);
            while (bt == BT_Water || bt == BT_Empty)
                bt = ws->GetBlockType(blockXstart + i, blockYstart + j - 1, --neighboorHeight);
            neighboorHeight++;

            if (neighboorHeight < height)
            {
                for (int k = neighboorHeight; k < height; k++)
                {
                    color = colors[ws->GetBlockType(blockXstart + i, blockYstart + j, k)];
                    PushFrontSide(vertices, indices, color, { (float)i, (float)j, (float)k });
                }
            }

            //BACK
            neighboorHeight = height;
            bt = ws->GetBlockType(blockXstart + i, blockYstart + j + 1, neighboorHeight);
            while (bt == BT_Water || bt == BT_Empty)
                bt = ws->GetBlockType(blockXstart + i, blockYstart + j + 1, --neighboorHeight);
            neighboorHeight++;

            if (neighboorHeight < height)
            {
                for (int k = neighboorHeight; k < height; k++)
                {
                    color = colors[ws->GetBlockType(blockXstart + i, blockYstart + j, k)];
                    PushBackSide(vertices, indices, color, { (float)i, (float)j, (float)k });
                }
            }
        }
    }

    ShiftEngine::IMeshManager * pMeshManager = ShiftEngine::GetContextManager()->GetMeshManager();
    auto landData = pMeshManager->CreateMeshFromVertices((uint8_t*)vertices.data(), vertices.size() * sizeof(PNC), indices, &nodeSemantic);
    if (!landData)
        return false;

    landNode->SetDataPtr(landData);
    landNode->SetBBox(bbox);

    vertices.clear();
    indices.clear();

    bbox.bMin.x = 0.0f;
    bbox.bMin.y = 0.0f;
    bbox.bMin.z = 1000.0f;

    bbox.bMax.x = (float)chunkWidth;
    bbox.bMax.y = (float)chunkWidth;

    const MathLib::Vector3F waterColor = { 0.3f, 0.3f, 1.0f };

    for (int i = 0; i < chunkWidth; i++)
    {
        for (int j = 0; j < chunkWidth; j++)
        {
            unsigned long curIndex = vertices.size();

            int height = ws->GetFullHeight(blockXstart + i, blockYstart + j);
            if ((float)height > bbox.bMax.z)
                bbox.bMax.z = (float)height;
            if ((float)height < bbox.bMin.z)
                bbox.bMin.z = (float)height;

            //UP

            auto blockType = ws->GetBlockType(blockXstart + i, blockYstart + j, (unsigned int)height - 1);
            if (blockType != BT_Water)
                continue;

            vertices.push_back(PNC(MathLib::Vector3F(i + 0.0f, j + 1.0f, (float)height), NormUP, waterColor));
            vertices.push_back(PNC(MathLib::Vector3F(i + 1.0f, j + 1.0f, (float)height), NormUP, waterColor));
            vertices.push_back(PNC(MathLib::Vector3F(i + 1.0f, j + 0.0f, (float)height), NormUP, waterColor));
            vertices.push_back(PNC(MathLib::Vector3F(i + 0.0f, j + 0.0f, (float)height), NormUP, waterColor));

            indices.push_back(curIndex + 0);
            indices.push_back(curIndex + 1);
            indices.push_back(curIndex + 2);

            indices.push_back(curIndex + 0);
            indices.push_back(curIndex + 2);
            indices.push_back(curIndex + 3);
        }
    }

    if (vertices.empty())
        return true;

    pMeshManager = ShiftEngine::GetContextManager()->GetMeshManager();
    auto waterData = pMeshManager->CreateMeshFromVertices((uint8_t*)vertices.data(), vertices.size() * sizeof(PNC), indices, &nodeSemantic);
    if (!waterData)
        return false;

    waterNode->SetDataPtr(waterData);
    waterNode->SetBBox(bbox);

    return true;
}

void WorldTesselator::SetCriticalSection(std::mutex * sect)
{
    criticalSection = sect;
}
