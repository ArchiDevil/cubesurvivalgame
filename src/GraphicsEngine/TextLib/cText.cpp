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

	long * indices = new long[mesh.indicesCount];
	TextPoint * vertices = new TextPoint[mesh.verticesCount];

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

		indices[indSh++] = vertSh;
		indices[indSh++] = vertSh + 1;
		indices[indSh++] = vertSh + 2;	//first triangle
		indices[indSh++] = vertSh;
		indices[indSh++] = vertSh + 2;
		indices[indSh++] = vertSh + 3;	//second triangle

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
		vertices[vertSh].tu		= (float) CharX / (float) fontPtr->Width;
		vertices[vertSh].tv		= (float) CharY / (float) fontPtr->Height;
		vertices[vertSh].x		= (float) CurX + OffsetX;
		vertices[vertSh++].y	= (float) OffsetY;

		//upper right
		vertices[vertSh].tu		= (float) (CharX + Width) / (float) fontPtr->Width;
		vertices[vertSh].tv		= (float) CharY / (float) fontPtr->Height;
		vertices[vertSh].x		= (float) Width + CurX + OffsetX;
		vertices[vertSh++].y	= (float) OffsetY;

		//lower right
		vertices[vertSh].tu		= (float) (CharX + Width) / (float) fontPtr->Width;
		vertices[vertSh].tv		= (float) (CharY + Height) / (float) fontPtr->Height;
		vertices[vertSh].x		= (float) Width + CurX + OffsetX;
		vertices[vertSh++].y	= (float) Height + OffsetY;

		//lower left
		vertices[vertSh].tu		= (float) CharX / (float) fontPtr->Width;
		vertices[vertSh].tv		= (float) (CharY + Height) / (float) fontPtr->Height;
		vertices[vertSh].x		= (float) CurX + OffsetX;
		vertices[vertSh++].y	= (float) Height + OffsetY;

		CurX += cp->XAdvance;
	}

	mesh.CreateBuffers(false, vertices, vertSh * sizeof(TextPoint), indices, indSh * sizeof(long), dev);

	mesh.vertexSize = sizeof(TextPoint);
	mesh.vertexSemantic = &ShiftEngine::plainSpriteVertexSemantic;
	mesh.vertexDeclaration = ShiftEngine::GetContextManager()->GetVertexDeclaration(ShiftEngine::plainSpriteVertexSemantic);

	delete [] indices;
	delete [] vertices;
}
