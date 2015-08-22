#pragma once

#include <string>

#include "IMeshData.h"
#include "ITexture.h"
#include "IProgram.h"
#include "VertexTypes.h"

#include <MathLib/math.h>

namespace ShiftEngine
{
    //2D sprite
    class Sprite
    {
    public:
        Sprite();
        Sprite(const std::wstring & filename,
            const MathLib::Vector2F & texCoordLeftTop = { 0.0f, 0.0f },
            const MathLib::Vector2F & texCoordRightBottom = { 1.0f, 1.0f });
        ~Sprite();

        void SetPosition(const MathLib::Vector2F & pos);
        void SetRotation(float rot);
        void SetScale(const MathLib::Vector2F & sc);
        void SetTexture(ShiftEngine::ITexturePtr ptr);
        void SetTexture(const std::wstring & filename);
        void SetMaskColor(const MathLib::Vector4F & color);

        void SetSizeInPixels(int x, int y);
        void Draw();

        MathLib::Vector2F GetTextureDimensions() const;

    private:
        void CreateBuffers(const MathLib::Vector2F & LT, const MathLib::Vector2F & RB);
        void LoadShader();

        ShiftEngine::ITexturePtr texture = nullptr;
        ShiftEngine::IMeshDataPtr spriteMesh = nullptr;

        MathLib::Vector4F MaskColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        MathLib::Vector2F Position = { 0.0f, 0.0f };
        float Rotation = 0.0f;
        MathLib::Vector2F Scale = { 1.0f, 1.0f };

        static ShiftEngine::IProgramPtr SpriteShader;
    };
}
