#pragma once

#include <D3D10.h>
#include <memory>

#include "MiscTypes.h"

namespace ShiftEngine
{
    class D3D10Texture
    {
        friend class Renderer;

        friend class D3D10ContextManager;
        friend class D3D10TextureManager;
        friend class D3D10Program;

    public:
        D3D10Texture(size_t width, size_t height, TextureType type = TextureType::Texture2D, ID3D10ShaderResourceView * Tex = nullptr);
        ~D3D10Texture();

        int GetHeight() const;
        int GetWidth() const;
        TextureType GetType() const;

    private:
        int width;
        int height;
        TextureType type;
        ID3D10ShaderResourceView * Tex;
    };

    typedef std::shared_ptr<D3D10Texture> TexturePtr;
}
