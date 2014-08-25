#include "../ShiftEngine.h"

#include "cText.h"

#include <cassert>

cText::cText( const std::string & textString, cFont * fp, ID3D10Device * dev ) 
	: fontPtr(fp)
{
	CreateAll(textString, dev);
}

cText::~cText() 
{
}

void cText::Draw( ID3D10Device * dev )
{
	mesh.vertexDeclaration->Bind();
	mesh.Draw(dev);
}

void cText::CreateAll( const std::string & str, ID3D10Device * dev )
{
	if(str.size() == 0)
		return;

	mesh.verticesCount = str.size() * 4;
	mesh.indicesCount = str.size() * 6;

	static std::vector<long> indicesPool(200);
	static std::vector<TextPoint> verticesPool(100);

	if (mesh.indicesCount > indicesPool.size())
		indicesPool.resize(mesh.indicesCount);

	if (mesh.verticesCount > verticesPool.size())
		verticesPool.resize(mesh.verticesCount);

	int vertSh = 0;
	int indSh = 0;

	int CurX = 0;

	for (unsigned int i = 0; i < str.size(); i++)
	{
		sChar * cp = fontPtr->GetCharacterPtr(str[i]);
		assert(cp->Height != 0);
		int CharX	= cp->x;
		int CharY	= cp->y;
		int Width	= cp->Width;
		int Height	= cp->Height;
		int OffsetX	= cp->XOffset;
		int OffsetY	= cp->YOffset;

		indicesPool[indSh++] = vertSh;
		indicesPool[indSh++] = vertSh + 1;
		indicesPool[indSh++] = vertSh + 2;	//first triangle
		indicesPool[indSh++] = vertSh;
		indicesPool[indSh++] = vertSh + 2;
		indicesPool[indSh++] = vertSh + 3;	//second triangle

		/*
		0   1
		-----
		|\  |
		| \ |
		|  \|
		-----
		3   2
		*/

		//upper left
		verticesPool[vertSh].tu	 = (float) CharX / (float) fontPtr->Width;
		verticesPool[vertSh].tv  = (float)CharY / (float)fontPtr->Height;
		verticesPool[vertSh].x   = (float)CurX + OffsetX;
		verticesPool[vertSh++].y = (float)OffsetY;

		//upper right
		verticesPool[vertSh].tu  = (float)(CharX + Width) / (float)fontPtr->Width;
		verticesPool[vertSh].tv  = (float)CharY / (float)fontPtr->Height;
		verticesPool[vertSh].x   = (float)Width + CurX + OffsetX;
		verticesPool[vertSh++].y = (float)OffsetY;

		//lower right
		verticesPool[vertSh].tu  = (float)(CharX + Width) / (float)fontPtr->Width;
		verticesPool[vertSh].tv  = (float)(CharY + Height) / (float)fontPtr->Height;
		verticesPool[vertSh].x   = (float)Width + CurX + OffsetX;
		verticesPool[vertSh++].y = (float)Height + OffsetY;

		//lower left
		verticesPool[vertSh].tu  = (float)CharX / (float)fontPtr->Width;
		verticesPool[vertSh].tv  = (float)(CharY + Height) / (float)fontPtr->Height;
		verticesPool[vertSh].x   = (float)CurX + OffsetX;
		verticesPool[vertSh++].y = (float)Height + OffsetY;

		CurX += cp->XAdvance;
	}

	mesh.CreateBuffers(false, verticesPool.data(), vertSh * sizeof(TextPoint), indicesPool.data(), indSh * sizeof(long), dev);

	mesh.vertexSize = sizeof(TextPoint);
	mesh.vertexSemantic = &ShiftEngine::plainSpriteVertexSemantic;
	mesh.vertexDeclaration = ShiftEngine::GetContextManager()->GetVertexDeclaration(ShiftEngine::plainSpriteVertexSemantic);
}
