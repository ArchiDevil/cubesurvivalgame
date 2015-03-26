#pragma once

#include <d3d10.h>

#include "cFont.h"
#include "../cMesh.h"

class cText   //non-unicode
{
public:
    cText(const std::string & textString, cFont * fp, ID3D10Device * dev);
    ~cText();

    void Draw(ID3D10Device * dev);

private:
    struct TextPoint
    {
        float x, y, tu, tv;
    };

    void CreateAll(const std::string & str, ID3D10Device * dev);

    ShiftEngine::MeshData mesh;
    cFont * fontPtr;

};
