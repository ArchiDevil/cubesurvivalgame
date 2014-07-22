#pragma once

#include <vector>

#include "MiscTypes.h"

namespace ShiftEngine
{
	enum ElemType
	{
		ET_FLOAT,
	};

	enum ElemSemantic
	{
		ES_Position,
		ES_Normal,
		ES_Binormal,
		ES_Tangent,
		ES_Texcoord,
		ES_Color,
		ES_Custom
	};

	enum VertexTypes
	{
		VT_P24_N24_T16_C8,	// Pos(3), Norm(3), Tc(2), Col(1)
		VT_P24_N24_T24_C8,	// Pos(3), Norm(3), Tc(3), Col(1)
		VT_P24_N24_C8,		// Pos(3), Norm(3), Col(1)
		VT_P16_T16_C8,		// Pos(2), Tc(2), Col(1)
		VT_End
		//add vertex types before VT_End
	};

	//vertex semantic
	struct VertexInfo
	{
		VertexInfo();
		VertexInfo(ElemType _type, unsigned _count, ElemSemantic _sem, const std::string & name = "");
		bool operator!=(const VertexInfo & ref) const;
		bool operator<(const VertexInfo & ref) const;

		ElemType type;
		unsigned count;
		ElemSemantic semantic;
		std::string name;
	};

	class VertexSemantic
	{
	public:
		void addSemantic(ElemType type,
								unsigned count,
								ElemSemantic semantic,
								const std::string & name = "");

		const std::vector<VertexInfo> & getInnerRepresentation() const;

		bool isNormalsHere() const;
		bool isTexcoordsHere() const;
		bool isVertexColorsHere() const;
		bool isTangentsHere() const;
		bool isBinormalsHere() const;

		bool operator<(const VertexSemantic & ref) const;

	private:
		std::vector<VertexInfo> vertexSemantics;
	};

	struct DefaultVertex
	{
		float Pos[3];
		float Normal[3];
		float Texcoord[2];
	};

	struct ExtendedVertex
	{
		float Pos[3];
		float Normal[3];
		float Texcoord[3];
		float Color;
	};

	struct ColorVertex
	{
		float Pos[3];
		float Normal[3];
		float Color;
	};

	struct PlainSpriteVertex
	{
		float Pos[2];
		float Texcoord[2];
		//float Color;
	};

	extern VertexSemantic colorVertexSemantic;
	extern VertexSemantic extendedVertexSemantic;
	extern VertexSemantic defaultVertexSemantic;
	extern VertexSemantic plainSpriteVertexSemantic;

}	//end of ShiftEngine namespace