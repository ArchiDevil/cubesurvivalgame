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
			const Vector2F & TexCoordLeftTop = Vector2F(0.0f, 0.0f), 
			const Vector2F & TexCoordRightBottom = Vector2F(1.0f, 1.0f));
		~Sprite();

		void SetPosition(const Vector2F & pos);				//������������� ������ � ����������� �������
		void SetRotation(float rot);					//������������ ������ �� ����
		void SetScale(const Vector2F & sc);					//������������� ������� �������
		void SetTexture(ShiftEngine::TexturePtr ptr);	//������������� �������� ��� �������
		void SetMaskColor(const Vector4F & color);			//������������� ���� ����� ��� �������

		void SetSizeInPixels(int x, int y);				//������������� ������ ������� � �������� �� ������
		void Draw();									//������ ������

		Vector2F GetTextureDimensions() const;			//���������� ������� �������� �������

	private:
		void CreateBuffers(const Vector2F & LT, const Vector2F & RB);
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
