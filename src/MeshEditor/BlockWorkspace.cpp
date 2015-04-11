#include "BlockWorkspace.h"

#include <cassert>
#include <vector>

#include <Utilities/ut.h>

MeshEditor::BlockWorkspace::BlockWorkspace(size_t x, size_t y, size_t z)
    : storage(x, y, z)
    , isBboxShown(false)
    , bbox(nullptr)
    , plane(nullptr)
    , mesh(nullptr)
    , tesselated(false)
{
    GridTexture = ShiftEngine::GetContextManager()->LoadTexture(L"gridCell.png");
    GeometryMaterial = ShiftEngine::Material(ShiftEngine::GetContextManager()->LoadShader(L"wsShaderGeometry.fx"));
    ColorMaterial = ShiftEngine::Material(ShiftEngine::GetContextManager()->LoadShader(L"wsShaderColor.fx"));
    GeometryMaterial.SetDiffuseTexture(GridTexture);

    semantic.addSemantic(ShiftEngine::ET_FLOAT, 3, ShiftEngine::ES_Position);
    semantic.addSemantic(ShiftEngine::ET_FLOAT, 3, ShiftEngine::ES_Normal);
    semantic.addSemantic(ShiftEngine::ET_FLOAT, 2, ShiftEngine::ES_Texcoord);
    semantic.addSemantic(ShiftEngine::ET_FLOAT, 3, ShiftEngine::ES_Color);
    ShiftEngine::GetContextManager()->GetVertexDeclaration(semantic);
}

MeshEditor::BlockWorkspace::~BlockWorkspace()
{
    if (light)
        ShiftEngine::GetSceneGraph()->RemoveDirectionalLightNode(light);
}

void MeshEditor::BlockWorkspace::Initialize()
{
    Tesselate();
    CreateBBox();
    CreatePlane();
    light = ShiftEngine::GetSceneGraph()->AddDirectionalLightNode(Vector3F(1.0f, 1.0f, -1.0f));
    LOG_INFO("Workspace initialized");
}

void MeshEditor::BlockWorkspace::Update()
{
    if (!tesselated)
        Tesselate();

    plane->SetScale(Vector3F(storage.GetHalfSize().x * 4.0f, storage.GetHalfSize().y * 4.0f, 1.0f));
    plane->SetPosition(Vector3F(storage.GetHalfSize().x - (float)storage.GetHalfSize().x * 2.0f, storage.GetHalfSize().y - (float)storage.GetHalfSize().y * 2.0f, 0.0f));

    //if(bboxShow)
    //	bbox->SetVisibility(true);
    //else
    //	bbox->SetVisibility(false);
}

MeshEditor::Block MeshEditor::BlockWorkspace::GetBlock(size_t x, size_t y, int z) const
{
    return storage.GetBlock(x, y, z);
}

#define maxShadowFactor2 23.1364880

float MeshEditor::BlockWorkspace::GetAOFactor(float x1, float x2, float y1, float y2, float z1, float z2)
{
    //checking maximum distance here

    const float maxDist = 2.1213f;
    float xc = x1 + abs(x1 - x2) / 2.0f;
    float yc = y1 + abs(y1 - y2) / 2.0f;
    float zc = z1 + abs(z1 - z2) / 2.0f;
    const Vector3F center = Vector3F(xc, yc, zc);
    float shadowFactor = 0.0f;
    // ONLY FOR 2 RADIUS AND 2.1213 MAX DISTANCE
    //float maxShadowFactor = maxShadowFactor2;

    //calculating maximum shadow factor
    float maxShadowFactor = 0.0f;
    for (float i = x1; i <= x2; i++)
        for (float j = y1; j <= y2; j++)
            for (float k = z1; k <= z2; k++)
                maxShadowFactor += pow(MathLib::distance(center, Vector3F(i, j, k)) / maxDist, 2);

    for (float i = x1; i <= x2; i++)
        for (float j = y1; j <= y2; j++)
            for (float k = z1; k <= z2; k++)
                if (GetBlock((int)i, (int)j, (int)k).exist)
                    shadowFactor += pow(MathLib::distance(center, Vector3F(i, j, k)) / maxDist, 2);

    //dividing
    return 1.0f - shadowFactor / maxShadowFactor;
}

void MeshEditor::BlockWorkspace::Tesselate()
{
    struct wVertex
    {
        wVertex(const Vector3F & pos, const Vector3F & normal, const Vector2F & tex, const Vector3F & col)
            : Position(pos)
            , Normal(normal)
            , Texcoord(tex)
            , Color(col)
        {
        }

        Vector3F Position;
        Vector3F Normal;
        Vector2F Texcoord;
        Vector3F Color;
    };

    std::vector<wVertex> vertices;
    std::vector<uint32_t> indices;
    unsigned long ind_index = 0;

    const Vector3F FRONT = Vector3F(-1.0f, 0.0f, 0.0f);
    const Vector3F BACK = Vector3F(1.0f, 0.0f, 0.0f);
    const Vector3F LEFT = Vector3F(0.0f, -1.0f, 0.0f);
    const Vector3F RIGHT = Vector3F(0.0f, 1.0f, 0.0f);
    const Vector3F DOWN = Vector3F(0.0f, 0.0f, -1.0f);
    const Vector3F UP = Vector3F(0.0f, 0.0f, 1.0f);

    const Vector3F sizes = storage.GetHalfSize() * 2.0f;

    for (float x = 0.0f; x < sizes.x; x++)
    {
        for (float y = 0.0f; y < sizes.y; y++)
        {
            for (float z = 0.0f; z < sizes.z; z++)
            {
                auto block = GetBlock((int)floor(x), (int)floor(y), (int)floor(z));
                if (block.exist)
                {
                    //check around
                    if (!GetBlock((int)floor(x - 1), (int)floor(y), (int)floor(z)).exist)
                    {
                        float AOFactor = GetAOFactor(x - 1, x - 1, y - 2, y + 1, z - 2, z + 1);
                        vertices.push_back(wVertex(Vector3F(x, y, z), FRONT, Vector2F(0.0f, 0.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 1, x - 1, y - 1, y + 2, z - 2, z + 1);
                        vertices.push_back(wVertex(Vector3F(x, y + 1, z), FRONT, Vector2F(1.0f, 0.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 1, x - 1, y - 2, y + 1, z - 1, z + 2);
                        vertices.push_back(wVertex(Vector3F(x, y, z + 1), FRONT, Vector2F(0.0f, 1.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 1, x - 1, y - 1, y + 2, z - 1, z + 2);
                        vertices.push_back(wVertex(Vector3F(x, y + 1, z + 1), FRONT, Vector2F(1.0f, 1.0f), block.color * AOFactor));

                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 2);
                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 1);
                        indices.push_back(ind_index + 3);

                        ind_index += 4;
                    }

                    if (!GetBlock((int)floor(x + 1), (int)floor(y), (int)floor(z)).exist)
                    {
                        float AOFactor = GetAOFactor(x + 1, x + 1, y - 2, y + 1, z - 2, z + 1);
                        vertices.push_back(wVertex(Vector3F(x + 1, y, z), BACK, Vector2F(0.0f, 0.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x + 1, x + 1, y - 1, y + 2, z - 2, z + 1);
                        vertices.push_back(wVertex(Vector3F(x + 1, y + 1, z), BACK, Vector2F(1.0f, 0.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x + 1, x + 1, y - 2, y + 1, z - 1, z + 2);
                        vertices.push_back(wVertex(Vector3F(x + 1, y, z + 1), BACK, Vector2F(0.0f, 1.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x + 1, x + 1, y - 1, y + 2, z - 1, z + 2);
                        vertices.push_back(wVertex(Vector3F(x + 1, y + 1, z + 1), BACK, Vector2F(1.0f, 1.0f), block.color * AOFactor));

                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 2);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 1);

                        ind_index += 4;
                    }

                    if (!GetBlock((int)floor(x), (int)floor(y - 1), (int)floor(z)).exist)
                    {
                        float AOFactor = GetAOFactor(x - 2, x + 1, y - 1, y - 1, z - 2, z + 1);
                        vertices.push_back(wVertex(Vector3F(x, y, z), LEFT, Vector2F(0.0f, 0.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 1, x + 2, y - 1, y - 1, z - 2, z + 1);
                        vertices.push_back(wVertex(Vector3F(x + 1, y, z), LEFT, Vector2F(1.0f, 0.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 2, x + 1, y - 1, y - 1, z - 1, z + 2);
                        vertices.push_back(wVertex(Vector3F(x, y, z + 1), LEFT, Vector2F(0.0f, 1.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 1, x + 2, y - 1, y - 1, z - 1, z + 2);
                        vertices.push_back(wVertex(Vector3F(x + 1, y, z + 1), LEFT, Vector2F(1.0f, 1.0f), block.color * AOFactor));

                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 2);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 1);

                        ind_index += 4;
                    }

                    if (!GetBlock((int)floor(x), (int)floor(y + 1), (int)floor(z)).exist)
                    {
                        float AOFactor = GetAOFactor(x - 2, x + 1, y + 1, y + 1, z - 2, z + 1);
                        vertices.push_back(wVertex(Vector3F(x, y + 1, z), RIGHT, Vector2F(0.0f, 0.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 1, x + 2, y + 1, y + 1, z - 2, z + 1);
                        vertices.push_back(wVertex(Vector3F(x + 1, y + 1, z), RIGHT, Vector2F(1.0f, 0.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 2, x + 1, y + 1, y + 1, z - 1, z + 2);
                        vertices.push_back(wVertex(Vector3F(x, y + 1, z + 1), RIGHT, Vector2F(0.0f, 1.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 1, x + 2, y + 1, y + 1, z - 1, z + 2);
                        vertices.push_back(wVertex(Vector3F(x + 1, y + 1, z + 1), RIGHT, Vector2F(1.0f, 1.0f), block.color * AOFactor));

                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 2);
                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 1);
                        indices.push_back(ind_index + 3);

                        ind_index += 4;
                    }

                    if (!GetBlock((int)floor(x), (int)floor(y), (int)floor(z - 1)).exist)
                    {
                        float AOFactor = GetAOFactor(x - 2, x + 1, y - 2, y + 1, z + 1, z - 1);
                        vertices.push_back(wVertex(Vector3F(x, y, z), DOWN, Vector2F(0.0f, 0.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 1, x + 2, y - 2, y + 1, z + 1, z - 1);
                        vertices.push_back(wVertex(Vector3F(x + 1, y, z), DOWN, Vector2F(1.0f, 0.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 2, x + 1, y - 1, y + 2, z + 1, z - 1);
                        vertices.push_back(wVertex(Vector3F(x, y + 1, z), DOWN, Vector2F(0.0f, 1.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 1, x + 2, y - 1, y + 2, z + 1, z - 1);
                        vertices.push_back(wVertex(Vector3F(x + 1, y + 1, z), DOWN, Vector2F(1.0f, 1.0f), block.color * AOFactor));

                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 2);
                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 1);
                        indices.push_back(ind_index + 3);

                        ind_index += 4;
                    }

                    if (!GetBlock((int)floor(x), (int)floor(y), (int)floor(z + 1)).exist)
                    {
                        float AOFactor = GetAOFactor(x - 2, x + 1, y - 2, y + 1, z + 1, z + 1);
                        vertices.push_back(wVertex(Vector3F(x, y, z + 1), UP, Vector2F(0.0f, 0.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 1, x + 2, y - 2, y + 1, z + 1, z + 1);
                        vertices.push_back(wVertex(Vector3F(x + 1, y, z + 1), UP, Vector2F(1.0f, 0.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 2, x + 1, y - 1, y + 2, z + 1, z + 1);
                        vertices.push_back(wVertex(Vector3F(x, y + 1, z + 1), UP, Vector2F(0.0f, 1.0f), block.color * AOFactor));

                        AOFactor = GetAOFactor(x - 1, x + 2, y - 1, y + 2, z + 1, z + 1);
                        vertices.push_back(wVertex(Vector3F(x + 1, y + 1, z + 1), UP, Vector2F(1.0f, 1.0f), block.color * AOFactor));

                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 2);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 0);
                        indices.push_back(ind_index + 3);
                        indices.push_back(ind_index + 1);

                        ind_index += 4;
                    }
                }
            }
        }
    }

    ShiftEngine::IMeshManager * pMeshManager = ShiftEngine::GetContextManager()->GetMeshManager();
    auto meshData = pMeshManager->CreateMeshFromVertices((uint8_t*)vertices.data(), vertices.size() * sizeof(wVertex), indices, &semantic);

    if (!mesh)
        mesh = ShiftEngine::GetSceneGraph()->AddMeshNode(meshData, MathLib::AABB({}, storage.GetHalfSize() * 2.0f), &GeometryMaterial);
    else
        mesh->SetDataPtr(meshData);

    tesselated = true;
}

void MeshEditor::BlockWorkspace::SetNotTesselated()
{
    tesselated = false;
}

void MeshEditor::BlockWorkspace::Save(const std::string & filename)
{
    struct Header
    {
        uint32_t size[3];
    };

    const MathLib::Vector3I sizes = storage.GetHalfSize() * 2.0f;
    Header h;
    h.size[0] = sizes.x;
    h.size[1] = sizes.y;
    h.size[2] = sizes.z;

    std::ofstream stream(filename);

    if (!stream || stream.fail() || !stream.is_open())
    {
        LOG_ERROR("Unable to save ", filename);
        return;
    }

    std::vector<uint8_t> buffer;
    storage.Serialize(buffer);

    stream.write((char*)&h, sizeof(Header));
    stream.write((char*)buffer.data(), buffer.size());
    stream.close();
}

void MeshEditor::BlockWorkspace::Load(const std::string & filename)
{
    struct Header
    {
        uint32_t size[3];
    };

    Header h;

    std::ifstream stream(filename);

    if (!stream || stream.fail() || !stream.is_open())
    {
        LOG_ERROR("Unable to load ", filename);
        return;
    }

    stream.read(reinterpret_cast<char*>(&h), sizeof(Header));

    std::vector<uint8_t> buffer(sizeof(Block) * h.size[0] * h.size[1] * h.size[2]);

    stream.read((char *)buffer.data(), buffer.size());
    stream.close();
    storage.Resize(h.size[0], h.size[1], h.size[2]);
    storage.Deserialize(buffer);
}

void MeshEditor::BlockWorkspace::Undo()
{
    if (actions.empty())
        return;

    auto &action = actions.top();
    action->Undo(&storage);
    actions.pop();
    Tesselate();
}

MathLib::Vector3F MeshEditor::BlockWorkspace::GetHalfSize() const
{
    return storage.GetHalfSize();
}

void MeshEditor::BlockWorkspace::CreatePlane()
{
    if (plane)
        return;

    ShiftEngine::DefaultVertex ver[] =
    {
        { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
    };
    long ind[6] = { 0, 1, 2, 0, 2, 3 };

    ShiftEngine::IMeshManager * pMeshManager = ShiftEngine::GetContextManager()->GetMeshManager();
    auto pMesh = pMeshManager->CreateMeshFromVertices((uint8_t*)ver, 4 * sizeof(ShiftEngine::DefaultVertex), { std::begin(ind), std::end(ind) }, &ShiftEngine::defaultVertexSemantic);
    ShiftEngine::MaterialPtr planeMtl = ShiftEngine::GetContextManager()->LoadMaterial(L"plane.mtl", L"editorPlane");
    plane = ShiftEngine::GetSceneGraph()->AddMeshNode(pMesh, MathLib::AABB(Vector3F(), Vector3F(1.0f, 1.0f, 0.0f)), planeMtl.get());
}

void MeshEditor::BlockWorkspace::CreateBBox()
{
    //if(!bbox)
    //{
    //	std::vector<ShiftEngine::InputElement> i;
    //	i.push_back(ShiftEngine::InputElement("POSITION", 3));
    //	i.push_back(ShiftEngine::InputElement("NORMAL", 3));
    //	i.push_back(ShiftEngine::InputElement("TEXCOORD", 2));
    //	ShiftEngine::cMaterial BboxShader = ShiftEngine::cMaterial(cGraphicsEngine::GetInstance().LoadShader(L"bbox.fx", i));
    //	bbox = ShiftEngine::SceneGraphInstance->AddStaticMeshNode(temp, MathLib::AABB(Vector3F(), Vector3F(1.0f, 1.0f, 1.0f)), BboxShader);

    //	Vector3F newScale = Vector3F((float)x_size + 0.1f, (float)y_size + 0.1f, (float)z_size + 0.1f);
    //	bbox->SetScale(newScale);
    //	Vector3F hp = Vector3F(this->GetHalfSize().x, this->GetHalfSize().y, 0.0f);
    //	bbox->SetPosition(hp);
    //}
    //else
    //{
    //	Vector3F newScale = Vector3F((float)x_size + 0.1f, (float)y_size + 0.1f, (float)z_size + 0.1f);
    //	bbox->SetScale(newScale);
    //	Vector3F hp = Vector3F(this->GetHalfSize().x, this->GetHalfSize().y, 0.0f);
    //	bbox->SetPosition(hp);
    //}
}

void MeshEditor::BlockWorkspace::ShowBoundingBox()
{
    isBboxShown = true;
}

void MeshEditor::BlockWorkspace::HideBoundingBox()
{
    isBboxShown = false;
}

bool MeshEditor::BlockWorkspace::IsBBoxShowed() const
{
    return isBboxShown;
}

int MeshEditor::BlockWorkspace::GetMaxSize() const
{
    return storage.GetMaxSize();
}

void MeshEditor::BlockWorkspace::VanishColor(bool flag)
{
    if (flag)
        mesh->SetMaterial(&GeometryMaterial);
    else
        mesh->SetMaterial(&ColorMaterial);
}

void MeshEditor::BlockWorkspace::AddBlock(size_t x, size_t y, size_t z)
{
    Brush b;
    b.startPos = Vector3I(x, y, z);
    b.type = Brush::BrushType::BT_BlockArray;
    b.size = { 1, 1, 1 };
    b.color = { 0.9f, 0.9f, 0.9f };

    actions.push(std::make_unique<AddBrushAction>(b));
    actions.top()->Execute(&storage);
    tesselated = false;
}

void MeshEditor::BlockWorkspace::RemoveBlock(size_t x, size_t y, size_t z)
{
    Brush b;
    b.startPos = Vector3I(x, y, z);
    b.type = Brush::BrushType::BT_BlockArray;
    b.size = { 1, 1, 1 };
    b.color = { 0.9f, 0.9f, 0.9f };

    actions.push(std::make_unique<RemoveBrushAction>(b));
    actions.top()->Execute(&storage);
    tesselated = false;
}

void MeshEditor::BlockWorkspace::SetBlockColor(size_t x, size_t y, size_t z, const Vector3F & color)
{
    actions.push(std::make_unique<SetBlockColorAction>(MathLib::Vector3I(x, y, z), color));
    actions.top()->Execute(&storage);
    tesselated = false;
}

void MeshEditor::BlockWorkspace::Resize(int Xup, int Yup, int Zup, int Xdown, int Ydown, int Zdown)
{
    actions.push(std::make_unique<ResizeAction>(MathLib::Vector3I(Xup, Yup, Zup), MathLib::Vector3I(Xdown, Ydown, Zdown)));
    actions.top()->Execute(&storage);
    tesselated = false;
    CreateBBox();
}

void MeshEditor::BlockWorkspace::ApplyBrushBlock(const Brush &b, bool remove)
{
    if (remove)
        actions.push(std::make_unique<AddBrushAction>(b));
    else
        actions.push(std::make_unique<RemoveBrushAction>(b));

    actions.top()->Execute(&storage);
    tesselated = false;
}
