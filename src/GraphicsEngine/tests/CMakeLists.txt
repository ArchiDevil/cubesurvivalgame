set(SOURCES 
            CommonTests.cpp
            TestCommon.h)

AddVSTest(GraphicsEngineUnitTests ${SOURCES})
VSTestLinkLibraries(GraphicsEngineUnitTests GraphicsEngine)
