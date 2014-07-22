#include "D3D10Texture.h"

ShiftEngine::D3D10Texture::D3D10Texture( size_t _width, size_t _height, TextureType _type, ID3D10ShaderResourceView * _Tex /*= nullptr*/ )
	: width(_width), height(_height), type(_type)
{
	this->Tex = _Tex;
}

ShiftEngine::D3D10Texture::~D3D10Texture()
{
	Tex->Release();
}

int ShiftEngine::D3D10Texture::GetHeight() const
{
	return height;
}

int ShiftEngine::D3D10Texture::GetWidth() const
{
	return width;
}

ShiftEngine::TextureType ShiftEngine::D3D10Texture::GetType() const
{
	return type;
}
