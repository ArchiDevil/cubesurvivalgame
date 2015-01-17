#include "VertexTypes.h"

namespace ShiftEngine
{
	VertexSemantic defaultVertexSemantic;
	VertexSemantic extendedVertexSemantic;
	VertexSemantic colorVertexSemantic;
	VertexSemantic plainSpriteVertexSemantic;
}

ShiftEngine::VertexInfo::VertexInfo()
	: type(ET_FLOAT)
	, count(1)
	, semantic(ES_Custom)
{
}

ShiftEngine::VertexInfo::VertexInfo( ElemType _type, unsigned _count, ElemSemantic _sem, const std::string & /*name*/ /*= ""*/ ) 
	: type(_type)
	, count(_count)
	, semantic(_sem)
{
}

bool ShiftEngine::VertexInfo::operator!=( const VertexInfo & ref ) const
{
	return type != ref.type
		|| count != ref.count
		|| semantic != ref.semantic
		|| name != ref.name;
}

bool ShiftEngine::VertexInfo::operator<( const VertexInfo & ref ) const
{
	if(type != ref.type) return type < ref.type;
	if(count != ref.count) return count < ref.count;
	if(semantic != ref.semantic) return semantic < ref.semantic;
	if(name != ref.name) return name < ref.name;
	return false;
}

void ShiftEngine::VertexSemantic::addSemantic( ElemType type, unsigned count, ElemSemantic semantic, const std::string & name /*= ""*/ )
{
	vertexSemantics.push_back(VertexInfo(type, count, semantic, name));
}

const std::vector<ShiftEngine::VertexInfo> & ShiftEngine::VertexSemantic::getInnerRepresentation() const
{
	return vertexSemantics;
}

bool ShiftEngine::VertexSemantic::operator<( const VertexSemantic & ref ) const
{
	if(vertexSemantics.size() != ref.vertexSemantics.size())
		return vertexSemantics.size() < ref.vertexSemantics.size();

	for(size_t i = 0; i < vertexSemantics.size(); ++i)
		if(vertexSemantics[i] != ref.vertexSemantics[i])
			return vertexSemantics[i] < ref.vertexSemantics[i];

	return false;
}

bool ShiftEngine::VertexSemantic::isNormalsHere() const
{
	for(auto elem : vertexSemantics)
		if(elem.semantic == ES_Normal)
			return true;

	return false;
}

bool ShiftEngine::VertexSemantic::isTexcoordsHere() const
{
	for(auto elem : vertexSemantics)
		if(elem.semantic == ES_Texcoord)
			return true;

	return false;
}

bool ShiftEngine::VertexSemantic::isVertexColorsHere() const
{
	for(auto elem : vertexSemantics)
		if(elem.semantic == ES_Color)
			return true;

	return false;
}

bool ShiftEngine::VertexSemantic::isTangentsHere() const
{
	for(auto elem : vertexSemantics)
		if(elem.semantic == ES_Tangent)
			return true;

	return false;
}

bool ShiftEngine::VertexSemantic::isBinormalsHere() const
{
	for(auto elem : vertexSemantics)
		if(elem.semantic == ES_Binormal)
			return true;

	return false;
}
