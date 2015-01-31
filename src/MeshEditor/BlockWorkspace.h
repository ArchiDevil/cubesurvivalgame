#pragma once

#include "WorkspaceActions.h"

#include <GraphicsEngine/ShiftEngine.h>
#include <MathLib/math.h>
#include <Utilities/IManager.h>

#include <stack>
#include <memory>

namespace MeshEditor
{
    class IAction;

    struct Block
    {
        Block() : exist(false) {}
        Vector3F color;
        bool exist;
    };

    class BlockWorkspace : public IManager
    {
    public:
        BlockWorkspace(int _x, int _y, int _z);
        ~BlockWorkspace();

        void Initialize();

        void ResizeWithoutSaved(int x_size, int y_size, int z_size);
        void Update();

        void AddBlock(int x, int y, int z);
        void RemoveBlock(int x, int y, int z);
        void SetBlockColor(int x, int y, int z, const Vector3F & color);
        Block & GetBlock(int x, int y, int z);
        Block GetBlock(int x, int y, int z) const;
        void Undo();

        void ShowBoundingBox();
        void HideBoundingBox();
        bool IsBBoxShowed() const;

        Vector3F GetHalfSize() const;

        void Save(const std::string & filename);
        void Load(const std::string & filename);

        void Resize(int Xup, int Yup, int Zup, int Xdown, int Ydown, int Zdown);
        int GetMaxSize() const;

        void VanishColor(bool flag);

    private:
        void SetNotTesselated();
        void Tesselate();
        int GetIndex(int x, int y, int z) const;
        int GetIndexNew(int x, int y, int z, int xSize, int ySize, int zSize);
        inline float GetAOFactor(float x1, float x2,
                                 float y1, float y2,
                                 float z1, float z2);
        void CreatePlane();
        void CreateBBox();

        bool tesselated;
        int x_size, y_size, z_size;
        Block * Elements;

        ShiftEngine::MeshNode * bbox;
        ShiftEngine::MeshNode * plane;
        ShiftEngine::MeshNode * mesh;

        ShiftEngine::Material ColorMaterial;
        ShiftEngine::Material GeometryMaterial;
        ShiftEngine::TexturePtr GridTexture;

        static ShiftEngine::VertexSemantic semantic;
        std::stack<std::unique_ptr<IAction>> actions;
        bool isBboxShown;
    };
}
