file(GLOB_RECURSE SRC *.cpp *.h)
AddVSTest(MathLibTests ${SRC})
VSTestLinkLibraries(MathLibTests MathLib)
