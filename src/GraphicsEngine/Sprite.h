#pragma once

#include <string>

#include <D3DX10.h>

#include "D3D10Texture.h"
#include "cMesh.h"
#include "Program.h"
#include "VertexTypes.h"

#include "../MathLib/math.h"

using namespace MathLib;

namespace ShiftEngine
{
	//2D sprite
	class Sprite
	{
	public:
		Sprite();
		Sprite(const std::wstring & _FileName, 
			Vector2F & TexCoordLeftTop = Vector2F(0.0f, 0.0f), 
			Vector2F & TexCoordRightBottom = Vector2F(1.0f, 1.0f));
		~Sprite();

		void SetPosition(Vector2F & pos);		//устанавливает спрайт в необходимую позицию
		void SetRotation(float rot);				//поворачивает спрайт на угол
		void SetScale(Vector2F & sc);			//устанавливает масштаб спрайта
		void SetTexture(ShiftEngine::TexturePtr ptr);			//устанавливает текстуру для спрайта
		void SetMaskColor(Vector4F & color);		//устанавливает цвет маски для спрайта

		void SetSizeInPixels(int x, int y);			//устанавливает размер спрайта в пикселях на экране
		void Draw();								//рисует спрайт

		Vector2F GetTextureDimensions() const;	//возвращает размеры текстуры спрайта

	private:
		void CreateBuffers(Vector2F & LT, Vector2F & RB);
		void LoadShader();

		ShiftEngine::TexturePtr texture;
		ShiftEngine::MeshDataPtr spriteMesh;

		Vector4F MaskColor;
		Vector2F Position;
		float Rotation;
		Vector2F Scale;

		static ShiftEngine::IProgramPtr SpriteShader;

	};
}
