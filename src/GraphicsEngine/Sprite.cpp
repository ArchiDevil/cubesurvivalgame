#include "Sprite.h"

#include "ShiftEngine.h"

#include <D3DX10.h>

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

    D3DXMATRIX mat, scale, pos, rot;
    D3DXMatrixTranslation(&pos, Position.x, Position.y, 0.0f);
    D3DXMatrixScaling(&scale, Scale.x, Scale.y, 0.0f);
    D3DXMatrixRotationZ(&rot, Rotation);
    D3DXMatrixOrthoOffCenterRH(&mat, 0.0, (float)settings.screenWidth, (float)settings.screenHeight, 0.0f, 0.0f, 1.0f);
    mat = (scale * rot * pos) * mat;

    SpriteShader->SetMatrixConstantByName("matRes", (float*)mat);
    SpriteShader->SetTextureByName("Texture", texture);
    SpriteShader->SetVectorConstantByName("MaskColor", MaskColor.ptr());
    SpriteShader->Apply(true);
    ShiftEngine::GetContextManager()->DrawMesh(spriteMesh);
}

void ShiftEngine::Sprite::SetTexture(ShiftEngine::TexturePtr ptr)
{
    texture = ptr;

    if (ptr)
        SetScale(Vector2F(1.0f, 1.0f));
}

void ShiftEngine::Sprite::SetTexture(const std::wstring & filename)
{
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
    PlainSpriteVertex ver[] =
    {
        { -0.5f, -0.5f, LT.x, LT.y },
        { 0.5f, -0.5f, RB.x, LT.y },
        { -0.5f, 0.5f, LT.x, RB.y },
        { 0.5f, 0.5f, RB.x, RB.y },
    };
    long ind[6] = { 0, 1, 2, 1, 3, 2 };

    MeshData * data = new MeshData();
    data->CreateBuffers(false, ver, sizeof(ver), ind, sizeof(ind), ShiftEngine::GetContextManager()->GetDevicePointer());

    data->indicesCount = 6;
    data->verticesCount = 4;
    data->vertexSize = sizeof(PlainSpriteVertex);

    data->vertexDeclaration = ShiftEngine::GetContextManager()->GetVertexDeclaration(ShiftEngine::plainSpriteVertexSemantic);
    data->vertexSemantic = &ShiftEngine::plainSpriteVertexSemantic;

    spriteMesh = MeshDataPtr(data);
}

void ShiftEngine::Sprite::SetSizeInPixels(int x, int y)
{
    if (texture)
        return;

    SetScale(Vector2F((float)x / texture->GetWidth(), (float)y / texture->GetHeight()));
}

void ShiftEngine::Sprite::LoadShader()
{
    if (!SpriteShader)
        SpriteShader = ShiftEngine::GetContextManager()->LoadShader(L"SpriteShader.fx");
}
