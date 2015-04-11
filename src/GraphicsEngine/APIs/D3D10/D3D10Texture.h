#pragma once

#include <D3D10.h>
#include <memory>

#include "../../ITexture.h"
#include "../../MiscTypes.h"

namespace ShiftEngine
{
    class D3D10Texture : public ITexture
    {
        friend class Renderer;

        friend class D3D10ContextManager;
        friend class D3D10TextureManager;
        friend class D3D10Program;

    public:
        D3D10Texture(ID3D10Device * pDevice, size_t width, size_t height, TextureType type = TextureType::Texture2D, ID3D10ShaderResourceView * texture = nullptr);
        ~D3D10Texture();

        void Bind(unsigned int index, BindingPoint point) override;

    private:
        ID3D10ShaderResourceView * texture;
        ID3D10Device * pDevice;
    };

    typedef std::shared_ptr<D3D10Texture> D3D10TexturePtr;
}
