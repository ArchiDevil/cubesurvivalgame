#pragma once

#include <d3d10.h>

#include "cFont.h"
#include "../APIs/D3D10/D3D10Mesh.h"

class cText   //non-unicode
{
public:
    cText(const std::string & textString, cFont * fp, ID3D10Device * dev);
    ~cText();

    void Draw();

private:
    struct TextPoint
    {
        float x, y, tu, tv;
    };

    void CreateAll(const std::string & str);

    ShiftEngine::D3D10MeshData mesh;
    cFont * fontPtr;

};
