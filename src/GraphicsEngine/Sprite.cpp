#include "Sprite.h"

#include "ShiftEngine.h"

ShiftEngine::IProgramPtr ShiftEngine::Sprite::SpriteShader;

ShiftEngine::Sprite::Sprite() 
	: Position(0.0f, 0.0f), Rotation(0.0f), Scale(1.0f, 1.0f), 
	MaskColor(1.0f, 1.0f, 1.0f, 1.0f), texture(nullptr)
{
	LoadShader();
	CreateBuffers(Vector2F(0.0f, 0.0f), Vector2F(1.0f, 1.0f));
}

ShiftEngine::Sprite::Sprite( const std::wstring & FileName, Vector2F & TexCoordLeftTop, Vector2F & TexCoordRightBottom ) 
	: Position(0.0f, 0.0f), Rotation(0.0f), Scale(1.0f, 1.0f), 
	MaskColor(1.0f, 1.0f, 1.0f, 1.0f)
{
	LoadShader();
	CreateBuffers(TexCoordLeftTop, TexCoordRightBottom);
	texture = ShiftEngine::GetContextManager()->LoadTexture(FileName);
	SetScale(Vector2F(1.0f, 1.0f));
}

ShiftEngine::Sprite::~Sprite()
{
	ShiftEngine::GetContextManager()->ResetPipeline();
}

void ShiftEngine::Sprite::Draw()
{
	if(!texture)
		return;

	D3DXMATRIX mat, scale, pos, rot;
	D3DXMatrixTranslation(&pos, Position.x, Position.y, 0.0f);
	D3DXMatrixScaling(&scale, Scale.x, Scale.y, 0.0f);
	D3DXMatrixRotationZ(&rot, Rotation);
	D3DXMatrixOrthoOffCenterRH(&mat, 0.0, 1024.0, 600.0, 0.0, 0.0, 1.0);
	mat = (scale * rot * pos) * mat;

	SpriteShader->SetMatrixConstantByName("matRes", (float*)mat);
	SpriteShader->SetTextureByName("Texture", texture);
	SpriteShader->SetVectorConstantByName("MaskColor", MaskColor.ptr());
	SpriteShader->Apply(true);
	ShiftEngine::GetContextManager()->DrawMesh(spriteMesh);
}

void ShiftEngine::Sprite::SetTexture( ShiftEngine::TexturePtr ptr )
{
	texture = ptr;

	if(ptr)
		SetScale(Vector2F(1.0f, 1.0f));
}

void ShiftEngine::Sprite::SetPosition( Vector2F & pos )
{
	Position = pos;
}

void ShiftEngine::Sprite::SetRotation( float rot )
{
	Rotation = rot;
}

void ShiftEngine::Sprite::SetScale( Vector2F & sc )
{
	Scale = sc;

	if(!texture)
		return;

	Scale.x *= texture->GetWidth();
	Scale.y *= texture->GetHeight();
}

Vector2F ShiftEngine::Sprite::GetTextureDimensions() const
{
	return Vector2F((float)texture->GetWidth(), (float)texture->GetHeight());
}

void ShiftEngine::Sprite::SetMaskColor( Vector4F & color )
{
	MaskColor = color;
}

void ShiftEngine::Sprite::CreateBuffers( Vector2F & LT, Vector2F & RB )
{
	PlainSpriteVertex ver[] = 
	{
		{-0.5f, -0.5f,	LT.x, LT.y},
		{0.5f, -0.5f,	RB.x, LT.y},
		{-0.5f, 0.5f,	LT.x, RB.y},
		{0.5f, 0.5f,	RB.x, RB.y},
	};
	long ind[6] = {0, 1, 2, 1, 3, 2};

	MeshData * data = new MeshData();
	data->CreateBuffers(false, ver, sizeof(ver), ind, sizeof(ind), ShiftEngine::GetContextManager()->GetDevicePointer());

	data->indicesCount = 6;
	data->verticesCount = 4;
	data->vertexSize = sizeof(PlainSpriteVertex);

	data->vertexDeclaration = ShiftEngine::GetContextManager()->GetVertexDeclaration(ShiftEngine::plainSpriteVertexSemantic);
	data->vertexSemantic = &ShiftEngine::plainSpriteVertexSemantic;

	spriteMesh = MeshDataPtr(data);
}

void ShiftEngine::Sprite::SetSizeInPixels( int x, int y )
{
	if(texture)
		SetScale(Vector2F((float)texture->GetWidth() / x, (float)texture->GetHeight() / y));
}

void ShiftEngine::Sprite::LoadShader()
{
	if(SpriteShader == nullptr)
		SpriteShader = ShiftEngine::GetContextManager()->LoadShader(L"SpriteShader.fx");
}
