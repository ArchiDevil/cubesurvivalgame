file(GLOB_RECURSE SRC *.cpp *.h)
AddVSTest(UtilitiesTests ${SRC})
VSTestLinkLibraries(UtilitiesTests Utilities)
