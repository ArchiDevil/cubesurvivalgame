set(SRC guiUnitTests.cpp)

set(LIBS GUI GraphicsEngine MathLib Utilities)

AddVSTest(GuiTests ${SRC})
VSTestLinkLibraries(GuiTests ${LIBS})
