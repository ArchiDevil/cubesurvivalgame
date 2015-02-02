#pragma once

#include "WorkspaceActions.h"
#include "BlockStorage.h"

#include <GraphicsEngine/ShiftEngine.h>
#include <MathLib/math.h>
#include <Utilities/IManager.h>

#include <stack>
#include <memory>

namespace MeshEditor
{
    class IAction;

    class BlockWorkspace final : public IManager
    {
    public:
        BlockWorkspace(size_t x, size_t y, size_t z);
        ~BlockWorkspace();

        void Initialize();
        void Update();

        void AddBlock(size_t x, size_t y, size_t z);
        void RemoveBlock(size_t x, size_t y, size_t z);
        void SetBlockColor(size_t x, size_t y, size_t z, const Vector3F & color);

        Block GetBlock(size_t x, size_t y, int z) const;
        void Undo();

        void ShowBoundingBox();
        void HideBoundingBox();
        bool IsBBoxShowed() const;

        void Save(const std::string & filename);
        void Load(const std::string & filename);

        void VanishColor(bool flag);
        void Resize(int Xup, int Yup, int Zup, int Xdown, int Ydown, int Zdown);

        Vector3F GetHalfSize() const;
        int GetMaxSize() const;

    private:
        void SetNotTesselated();
        void Tesselate();
        inline float GetAOFactor(float x1, float x2,
                                 float y1, float y2,
                                 float z1, float z2);
        void CreatePlane();
        void CreateBBox();

        bool tesselated;
        BlockStorage storage;

        ShiftEngine::MeshNode * bbox;
        ShiftEngine::MeshNode * plane;
        ShiftEngine::MeshNode * mesh;
        ShiftEngine::LightNode * light;

        ShiftEngine::Material ColorMaterial;
        ShiftEngine::Material GeometryMaterial;
        ShiftEngine::TexturePtr GridTexture;

        ShiftEngine::VertexSemantic semantic;
        std::stack<std::unique_ptr<IAction>> actions;
        bool isBboxShown;
    };
}
