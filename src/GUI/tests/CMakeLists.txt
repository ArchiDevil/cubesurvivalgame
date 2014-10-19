file(GLOB_RECURSE SRC *.cpp *.h)
set(LIBS GUI GraphicsEngine MathLib Utilities)
AddVSTest(GuiTests ${SRC})
VSTestLinkLibraries(GuiTests ${LIBS})
