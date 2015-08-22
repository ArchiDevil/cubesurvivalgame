#include "Sprite.h"

#include "ShiftEngine.h"

using namespace MathLib;

ShiftEngine::IProgramPtr ShiftEngine::Sprite::SpriteShader;

ShiftEngine::Sprite::Sprite()
    : Sprite{ L"" }
{
}

ShiftEngine::Sprite::Sprite(const std::wstring & filename, const Vector2F & texCoordLeftTop, const Vector2F & texCoordRightBottom)
{
    LoadShader();
    CreateBuffers(texCoordLeftTop, texCoordRightBottom);
    if (!filename.empty())
        SetTexture(filename);
}

ShiftEngine::Sprite::~Sprite()
{
    ShiftEngine::GetContextManager()->ResetPipeline();
}

void ShiftEngine::Sprite::Draw()
{
    if (!texture)
        return;

    auto settings = ShiftEngine::GetContextManager()->GetEngineSettings();

    mat4f mat, scale, pos, rot;
    pos = matrixTranslation(Position.x, Position.y, 0.0f);
    scale = matrixScaling(Scale.x, Scale.y, 0.0f);
    rot = matrixRotationZ(Rotation);
    mat = matrixOrthoOffCenterLH<float, 4>(0.0, (float)settings.screenWidth, (float)settings.screenHeight, 0.0f, 0.0f, 1.0f);
    mat = (scale * rot * pos) * mat;

    SpriteShader->SetMatrixConstantByName("matRes", (float*)mat);
    SpriteShader->SetTextureByName("Texture", texture);
    SpriteShader->SetVectorConstantByName("MaskColor", MaskColor.ptr());
    SpriteShader->Apply(true);
    ShiftEngine::GetContextManager()->DrawMesh(spriteMesh);
}

void ShiftEngine::Sprite::SetTexture(ShiftEngine::ITexturePtr ptr)
{
    texture = ptr;

    if (ptr)
        SetScale(Vector2F(1.0f, 1.0f));
}

void ShiftEngine::Sprite::SetTexture(const std::wstring & filename)
{
    if (filename.empty())
        texture = nullptr;
    else
        texture = ShiftEngine::GetContextManager()->LoadTexture(filename);
    SetScale(Vector2F(1.0f, 1.0f));
}

void ShiftEngine::Sprite::SetPosition(const Vector2F & pos)
{
    Position = pos;
}

void ShiftEngine::Sprite::SetRotation(float rot)
{
    Rotation = rot;
}

void ShiftEngine::Sprite::SetScale(const Vector2F & sc)
{
    Scale = sc;

    if (!texture)
        return;

    Scale.x *= texture->GetWidth();
    Scale.y *= texture->GetHeight();
}

Vector2F ShiftEngine::Sprite::GetTextureDimensions() const
{
    if (!texture)
        return{ 0.0f, 0.0f };
    return Vector2F((float)texture->GetWidth(), (float)texture->GetHeight());
}

void ShiftEngine::Sprite::SetMaskColor(const Vector4F & color)
{
    MaskColor = color;
}

void ShiftEngine::Sprite::CreateBuffers(const Vector2F & LT, const Vector2F & RB)
{
    std::vector<PlainSpriteVertex> ver(4);
    ver[0] = { { -0.5f, -0.5f }, { LT.x, LT.y } };
    ver[1] = { { 0.5f, -0.5f }, { RB.x, LT.y } };
    ver[2] = { { -0.5f, 0.5f }, { LT.x, RB.y } };
    ver[3] = { { 0.5f, 0.5f }, { RB.x, RB.y } };

    std::vector<uint32_t> ind = { 0, 1, 2, 1, 3, 2 };

    IMeshManager * pMeshManager = GetContextManager()->GetMeshManager();
    spriteMesh = pMeshManager->CreateMeshFromVertices(
        (uint8_t*)ver.data(),
        ver.size() * sizeof(PlainSpriteVertex),
        ind,
        &ShiftEngine::plainSpriteVertexSemantic,
        {});
}

void ShiftEngine::Sprite::SetSizeInPixels(int x, int y)
{
    if (!texture)
        return;

    SetScale(Vector2F((float)x / texture->GetWidth(), (float)y / texture->GetHeight()));
}

void ShiftEngine::Sprite::LoadShader()
{
    if (!SpriteShader)
        SpriteShader = ShiftEngine::GetContextManager()->LoadShader(L"SpriteShader.fx");
}
