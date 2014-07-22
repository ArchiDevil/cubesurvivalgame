#pragma once

#include <string>

#include "block.h"

class blockInfo
{
public:
	std::wstring GetTextureName() const { return textureName; }
	void SetTextureName(std::wstring val) { textureName = val; }

	void setTextureIndex(int index) {textureIndex = index;}
	int getTextureIndex() const {return textureIndex;}

private:
	int textureIndex;			//only one at one time
	std::wstring textureName;	//only one at one time
};