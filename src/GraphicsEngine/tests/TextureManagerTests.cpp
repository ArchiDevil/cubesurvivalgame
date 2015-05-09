#include "CppUnitTest.h"
#include "TestCommon.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ShiftEngine;
using namespace MathLib;

namespace UnitTests
{
    TEST_CLASS(GraphicsEngineUnitTests)
    {
        //ID3D10Device* pDevice;

        //bool InitDevice()
        //{
        //    if (FAILED(D3D10CreateDevice(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, NULL, D3D10_SDK_VERSION, &pDevice)))
        //    {
        //        if (FAILED(D3D10CreateDevice(NULL, D3D10_DRIVER_TYPE_SOFTWARE, NULL, NULL, D3D10_SDK_VERSION, &pDevice)))
        //        {
        //            return false;
        //        }
        //        return true;
        //    }
        //    else
        //    {
        //        return true;
        //    }
        //    return false;
        //}

        //void TerminateDevice()
        //{
        //    if (pDevice)
        //        pDevice->Release();
        //}

    public:
        //GraphicsEngineUnitTests() : pDevice(nullptr) {}

        //TEST_METHOD(TextureManagerTest)
        //{
        //    //this method tests that InitDevice() result is true, otherwise it will print message and fail current test
        //    //every test that needs device pointer must call this method
        //    Assert::IsTrue(InitDevice(), L"Unable to init device");

        //    //test body must be written here

        //    //assume that this method is invoked at the end of every test
        //    TerminateDevice();
        //}
    };
}
