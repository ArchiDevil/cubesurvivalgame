#include "CppUnitTest.h"
#include "TestCommon.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ShiftEngine;
using namespace MathLib;

namespace UnitTests
{
    TEST_CLASS(GraphicsEngineCommonTests)
    {
        void InitFrustum(CameraFrustum & frustum, const vec3<float> & pos, const vec3<float> & upDir, const vec3<float> & lookDir)
        {
            mat4f matView, matProj;
            matView = matrixLookAtRH<float, 4>(pos, lookDir, upDir);
            matProj = matrixPerspectiveFovRH<float, 4>(M_PIF * 60.0f / 180.0f, 800.0f / 600.0f, 1.0f, 100.0f);
            frustum.BuildFrustum(matView, matProj);
        }

    public:
        TEST_METHOD(CameraFrustumBaseAxisTest)
        {
            CameraFrustum frustum;
            InitFrustum(frustum, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, { 1.0f, 0.0f, 0.0f });
            CameraFrustum::CullingStatus status = frustum.CheckAABB(AABB({ 50.0f, -1.0f, -1.0f }, { 52.0f, 1.0f, 1.0f }));
            Assert::IsTrue(status > CameraFrustum::CS_Out);

            InitFrustum(frustum, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f });
            status = frustum.CheckAABB(AABB({ -52.0f, -1.0f, -1.0f }, { -50.0f, 1.0f, 1.0f }));
            Assert::IsTrue(status > CameraFrustum::CS_Out);

            InitFrustum(frustum, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
            status = frustum.CheckAABB(AABB({ -1.0f, 50.0f, -1.0f }, { 1.0f, 52.0f, 1.0f }));
            Assert::IsTrue(status > CameraFrustum::CS_Out);

            InitFrustum(frustum, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f });
            status = frustum.CheckAABB(AABB({ -1.0f, -52.0f, -1.0f }, { 1.0f, -50.0f, 1.0f }));
            Assert::IsTrue(status > CameraFrustum::CS_Out);
        }

        TEST_METHOD(CameraFrustumOOBBTest_NOT_IMPLEMENTED)
        {
            //CullingStatus CheckOOBB(const MathLib::OOBB & bbox) const;
        }

        TEST_METHOD(CameraFrustumQTreeTest_NOT_IMPLEMENTED)
        {
            //CullingStatus CheckQTreeNode(const MathLib::AABB & bbox) const;
        }
    };
}
