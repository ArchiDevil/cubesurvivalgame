#include "Utils.h"

#include "ShiftEngine.h"

#include <fstream>

bool ShiftEngine::Utilities::getVerticesFromFile(const std::wstring & filename,
                                                 SerializedLIM & vertices,
                                                 std::vector<uint32_t> & indices)
{
    std::ifstream in(filename.c_str(), std::ios::binary);

    if (!in || in.fail())
        return false;

    MeshLIMHeader head;
    in.read(reinterpret_cast<char*>(&head), sizeof(MeshLIMHeader));
    if (head.VERSION != LIM_HEADER_VERSION)
        return false;

    unsigned int size = head.verticesCount;

    vertices.position.resize(size);
    in.read(reinterpret_cast<char*>(vertices.position.data()), sizeof(float) * 3 * size);

    if (head.hasNormals)
    {
        vertices.normal.resize(size);
        in.read(reinterpret_cast<char*>(vertices.normal.data()), sizeof(float) * 3 * size);
    }
    if (head.hasTexCoords)
    {
        vertices.texcoord.resize(size);
        in.read(reinterpret_cast<char*>(vertices.texcoord.data()), sizeof(float) * 2 * size);
    }
    if (head.hasColors)
    {
        vertices.colors.resize(size);
        in.read(reinterpret_cast<char*>(vertices.colors.data()), sizeof(float) * 3 * size);
    }

    int sizeInd = head.indicesCount;
    indices.resize(sizeInd);

    in.read(reinterpret_cast<char*>(indices.data()), sizeof(long) * sizeInd);
    in.close();
    return true;
}

ShiftEngine::IMeshDataPtr ShiftEngine::Utilities::createCube()
{
    static IMeshDataPtr pDefaultMesh = nullptr;
    if (pDefaultMesh != nullptr)
        return pDefaultMesh;
    //some actions
    std::vector<DefaultVertex> vertices(24);
    vertices[0] = { { 0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } };
    vertices[1] = { { 0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } };
    vertices[2] = { { -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } };
    vertices[3] = { { -0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } };

    vertices[4] = { { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } };
    vertices[5] = { { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } };
    vertices[6] = { { -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } };
    vertices[7] = { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } };

    vertices[8] = { { -0.5f, -0.5f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } };
    vertices[9] = { { 0.5f, -0.5f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } };
    vertices[10] = { { 0.5f, -0.5f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } };
    vertices[11] = { { -0.5f, -0.5f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } };

    vertices[12] = { { -0.5f, 0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } };
    vertices[13] = { { 0.5f, 0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } };
    vertices[14] = { { 0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } };
    vertices[15] = { { -0.5f, 0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } };

    vertices[16] = { { -0.5f, 0.5f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };
    vertices[17] = { { -0.5f, -0.5f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };
    vertices[18] = { { -0.5f, -0.5f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };
    vertices[19] = { { -0.5f, 0.5f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };

    vertices[20] = { { 0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } };
    vertices[21] = { { 0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } };
    vertices[22] = { { 0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } };
    vertices[23] = { { 0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } };

    std::vector<uint32_t> indices =
    {
        //up
        0, 1, 2,
        2, 1, 3,
        //down +4
        5, 4, 7,
        7, 4, 6,
        //left +8
        8, 9, 10,
        8, 10, 11,
        //right +12
        14, 13, 12,
        15, 14, 12,
        //front +16
        16, 17, 19,
        17, 18, 19,
        //back +20
        23, 21, 20,
        23, 22, 21
    };

    IMeshManager * pMeshManager = ShiftEngine::GetContextManager()->GetMeshManager();
    pDefaultMesh = pMeshManager->CreateMeshFromVertices((uint8_t*)vertices.data(), vertices.size() * sizeof(DefaultVertex), indices, &defaultVertexSemantic);
    return pDefaultMesh;
}

ShiftEngine::IMeshDataPtr ShiftEngine::Utilities::createPlane()
{
    static IMeshDataPtr pDefaultMesh = nullptr;
    if (pDefaultMesh != nullptr)
        return pDefaultMesh;

    std::vector<DefaultVertex> vertices(4);
    vertices[0] = { { 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } };
    vertices[1] = { { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } };
    vertices[2] = { { -0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } };
    vertices[3] = { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } };

    std::vector<uint32_t> indices =
    {
        //up
        0, 1, 2,
        2, 1, 3,
    };

    IMeshManager * pMeshManager = ShiftEngine::GetContextManager()->GetMeshManager();
    pDefaultMesh = pMeshManager->CreateMeshFromVertices((uint8_t*)vertices.data(), vertices.size() * sizeof(DefaultVertex), indices, &defaultVertexSemantic);
    return pDefaultMesh;
}

ShiftEngine::IMeshDataPtr ShiftEngine::Utilities::createSphere()
{
    static IMeshDataPtr pDefaultMesh = nullptr;
    if (pDefaultMesh != nullptr)
        return pDefaultMesh;

    const int subdivisions = 36;
    //const float radius = 1.0f;

    std::vector<DefaultVertex> vertices(subdivisions * subdivisions * 6);
    memset(vertices.data(), 0, sizeof(DefaultVertex) * vertices.size());

    const float step = (float)M_PI / subdivisions;

    for (int i = 0; i < subdivisions; i++) //horizontal step (phi)
    {
        for (int j = 0; j < subdivisions; j++) //vertical step (theta)
        {
            float sinPhi[] = { sin(2.0f * step * i), sin(2.0f * step * (i + 1)) };
            float cosPhi[] = { cos(2.0f * step * i), cos(2.0f * step * (i + 1)) };
            float sinTheta[] = { sin(step * j), sin(step * (j + 1)) };
            float cosTheta[] = { cos(step * j), cos(step * (j + 1)) };

            //create 4 vertices patch
            MathLib::Vector3F patch[4];
            patch[0] = { sinTheta[1] * cosPhi[0], sinTheta[1] * sinPhi[0], cosTheta[1] };
            patch[1] = { sinTheta[1] * cosPhi[1], sinTheta[1] * sinPhi[1], cosTheta[1] };
            patch[2] = { sinTheta[0] * cosPhi[1], sinTheta[0] * sinPhi[1], cosTheta[0] };
            patch[3] = { sinTheta[0] * cosPhi[0], sinTheta[0] * sinPhi[0], cosTheta[0] };

            MathLib::Vector2F texcoords[4];
            texcoords[0] = { (float)i / subdivisions, (float)(j + 1) / subdivisions };
            texcoords[1] = { (float)(i + 1) / subdivisions, (float)(j + 1) / subdivisions };
            texcoords[2] = { (float)(i + 1) / subdivisions, (float)j / subdivisions };
            texcoords[3] = { (float)i / subdivisions, (float)j / subdivisions };

            memcpy(vertices[i * subdivisions * 6 + j * 6].Pos, patch[0].ptr(), sizeof(MathLib::Vector3F));
            memcpy(vertices[i * subdivisions * 6 + j * 6].Normal, patch[0].ptr(), sizeof(MathLib::Vector3F));
            memcpy(vertices[i * subdivisions * 6 + j * 6].Texcoord, texcoords[0].ptr(), sizeof(MathLib::Vector2F));

            memcpy(vertices[i * subdivisions * 6 + j * 6 + 2].Pos, patch[1].ptr(), sizeof(MathLib::Vector3F));
            memcpy(vertices[i * subdivisions * 6 + j * 6 + 2].Normal, patch[1].ptr(), sizeof(MathLib::Vector3F));
            memcpy(vertices[i * subdivisions * 6 + j * 6 + 2].Texcoord, texcoords[1].ptr(), sizeof(MathLib::Vector2F));

            memcpy(vertices[i * subdivisions * 6 + j * 6 + 1].Pos, patch[2].ptr(), sizeof(MathLib::Vector3F));
            memcpy(vertices[i * subdivisions * 6 + j * 6 + 1].Normal, patch[2].ptr(), sizeof(MathLib::Vector3F));
            memcpy(vertices[i * subdivisions * 6 + j * 6 + 1].Texcoord, texcoords[2].ptr(), sizeof(MathLib::Vector2F));

            memcpy(vertices[i * subdivisions * 6 + j * 6 + 3].Pos, patch[0].ptr(), sizeof(MathLib::Vector3F));
            memcpy(vertices[i * subdivisions * 6 + j * 6 + 3].Normal, patch[0].ptr(), sizeof(MathLib::Vector3F));
            memcpy(vertices[i * subdivisions * 6 + j * 6 + 3].Texcoord, texcoords[0].ptr(), sizeof(MathLib::Vector2F));

            memcpy(vertices[i * subdivisions * 6 + j * 6 + 5].Pos, patch[2].ptr(), sizeof(MathLib::Vector3F));
            memcpy(vertices[i * subdivisions * 6 + j * 6 + 5].Normal, patch[2].ptr(), sizeof(MathLib::Vector3F));
            memcpy(vertices[i * subdivisions * 6 + j * 6 + 5].Texcoord, texcoords[2].ptr(), sizeof(MathLib::Vector2F));

            memcpy(vertices[i * subdivisions * 6 + j * 6 + 4].Pos, patch[3].ptr(), sizeof(MathLib::Vector3F));
            memcpy(vertices[i * subdivisions * 6 + j * 6 + 4].Normal, patch[3].ptr(), sizeof(MathLib::Vector3F));
            memcpy(vertices[i * subdivisions * 6 + j * 6 + 4].Texcoord, texcoords[3].ptr(), sizeof(MathLib::Vector2F));
        }
    }

    IMeshManager * pMeshManager = ShiftEngine::GetContextManager()->GetMeshManager();
    pDefaultMesh = pMeshManager->CreateMeshFromVertices((uint8_t*)vertices.data(), vertices.size() * sizeof(DefaultVertex), {}, &defaultVertexSemantic);
    return pDefaultMesh;
}

void ShiftEngine::Utilities::convertVertices(const std::vector<DefaultVertex> & input, std::vector<ExtendedVertex> & output)
{
    output.resize(input.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        output[i].Pos[0] = input[i].Pos[0];
        output[i].Pos[1] = input[i].Pos[1];
        output[i].Pos[2] = input[i].Pos[2];
        output[i].Normal[0] = input[i].Normal[0];
        output[i].Normal[1] = input[i].Normal[1];
        output[i].Normal[2] = input[i].Normal[2];
        output[i].Texcoord[0] = input[i].Texcoord[0];
        output[i].Texcoord[1] = input[i].Texcoord[1];
    }
}

void ShiftEngine::Utilities::convertVertices(const std::vector<DefaultVertex> & input, std::vector<ColorVertex> & output)
{
    output.resize(input.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        output[i].Pos[0] = input[i].Pos[0];
        output[i].Pos[1] = input[i].Pos[1];
        output[i].Pos[2] = input[i].Pos[2];
        output[i].Normal[0] = input[i].Normal[0];
        output[i].Normal[1] = input[i].Normal[1];
        output[i].Normal[2] = input[i].Normal[2];
    }
}

void ShiftEngine::Utilities::convertVertices(const std::vector<ExtendedVertex> & input, std::vector<DefaultVertex> & output)
{
    output.resize(input.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        output[i].Pos[0] = input[i].Pos[0];
        output[i].Pos[1] = input[i].Pos[1];
        output[i].Pos[2] = input[i].Pos[2];
        output[i].Normal[0] = input[i].Normal[0];
        output[i].Normal[1] = input[i].Normal[1];
        output[i].Normal[2] = input[i].Normal[2];
        output[i].Texcoord[0] = input[i].Texcoord[0];
        output[i].Texcoord[1] = input[i].Texcoord[1];
    }
}

void ShiftEngine::Utilities::convertVertices(const std::vector<ExtendedVertex> & input, std::vector<ColorVertex> & output)
{
    output.resize(input.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        output[i].Pos[0] = input[i].Pos[0];
        output[i].Pos[1] = input[i].Pos[1];
        output[i].Pos[2] = input[i].Pos[2];
        output[i].Normal[0] = input[i].Normal[0];
        output[i].Normal[1] = input[i].Normal[1];
        output[i].Normal[2] = input[i].Normal[2];
        output[i].Color[0] = input[i].Color[0];
        output[i].Color[1] = input[i].Color[1];
        output[i].Color[2] = input[i].Color[2];
    }
}

void ShiftEngine::Utilities::convertVertices(const std::vector<ColorVertex> & input, std::vector<DefaultVertex> & output)
{
    output.resize(input.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        output[i].Pos[0] = input[i].Pos[0];
        output[i].Pos[1] = input[i].Pos[1];
        output[i].Pos[2] = input[i].Pos[2];
        output[i].Normal[0] = input[i].Normal[0];
        output[i].Normal[1] = input[i].Normal[1];
        output[i].Normal[2] = input[i].Normal[2];
    }
}

void ShiftEngine::Utilities::convertVertices(const std::vector<ColorVertex> & input, std::vector<ExtendedVertex> & output)
{
    output.resize(input.size());
    for (size_t i = 0; i < input.size(); ++i)
    {
        output[i].Pos[0] = input[i].Pos[0];
        output[i].Pos[1] = input[i].Pos[1];
        output[i].Pos[2] = input[i].Pos[2];
        output[i].Normal[0] = input[i].Normal[0];
        output[i].Normal[1] = input[i].Normal[1];
        output[i].Normal[2] = input[i].Normal[2];
        output[i].Color[0] = input[i].Color[0];
        output[i].Color[1] = input[i].Color[1];
        output[i].Color[2] = input[i].Color[2];
    }
}
