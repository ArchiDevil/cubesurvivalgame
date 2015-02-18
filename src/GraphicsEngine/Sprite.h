#pragma once

#include <string>

#include "D3D10Texture.h"
#include "cMesh.h"
#include "Program.h"
#include "VertexTypes.h"

#include <MathLib/math.h>

using namespace MathLib;

namespace ShiftEngine
{
    //2D sprite
    class Sprite
    {
    public:
        Sprite();
        Sprite(const std::wstring & filename,
            const Vector2F & texCoordLeftTop = { 0.0f, 0.0f },
            const Vector2F & texCoordRightBottom = { 1.0f, 1.0f });
        ~Sprite();

        void SetPosition(const Vector2F & pos);
        void SetRotation(float rot);
        void SetScale(const Vector2F & sc);
        void SetTexture(ShiftEngine::TexturePtr ptr);
        void SetTexture(const std::wstring & filename);
        void SetMaskColor(const Vector4F & color);

        void SetSizeInPixels(int x, int y);
        void Draw();

        Vector2F GetTextureDimensions() const;

    private:
        void CreateBuffers(const Vector2F & LT, const Vector2F & RB);
        void LoadShader();

        ShiftEngine::TexturePtr texture = nullptr;
        ShiftEngine::MeshDataPtr spriteMesh = nullptr;

        Vector4F MaskColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        Vector2F Position = { 0.0f, 0.0f };
        float Rotation = 0.0f;
        Vector2F Scale = { 1.0f, 1.0f };

        static ShiftEngine::IProgramPtr SpriteShader;
    };
}
