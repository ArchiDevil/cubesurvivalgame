#pragma once

#include <D3D10.h>
#include <memory>

#include "MiscTypes.h"

namespace ShiftEngine
{
	class D3D10Texture
	{
		friend class D3D10ContextManager;
		friend class Renderer;
		friend class D3D10TextureManager;

	public:
		D3D10Texture(size_t _width, size_t _height, TextureType _type = TextureType::Texture2D, ID3D10ShaderResourceView * _Tex = nullptr);
		~D3D10Texture();

		int GetHeight() const;
		int GetWidth() const;
		TextureType GetType() const;

		ID3D10ShaderResourceView * Tex;

	private:
		int width;
		int height;
		TextureType type;
	};

	typedef std::shared_ptr<D3D10Texture> TexturePtr;
}