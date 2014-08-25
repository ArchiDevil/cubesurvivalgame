cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SRC *.cpp *.h)

# ugly hacking shit
string(REGEX REPLACE "/Common7/IDE/devenv.com" "/VC" VCINSTALLDIR ${CMAKE_MAKE_PROGRAM})

include_directories(${VCINSTALLDIR}/UnitTest/include)
link_directories(${VCINSTALLDIR}/UnitTest/lib)
include_directories($ENV{DXSDK_DIR}/include)
link_directories($ENV{DXSDK_DIR}/lib/x86)
add_library(UnitTests SHARED ${SRC})
target_link_libraries(UnitTests GraphicsEngine GUI Utilities MathLib)

set_target_properties(UnitTests PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_target_properties(UnitTests PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
set_target_properties(UnitTests PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
set_target_properties(UnitTests PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")

cmake_minimum_required(VERSION 2.6)
project(basic_test)

################################
# GTest
################################
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add test cpp file
add_executable( runUnitTests testgtest.cpp )
# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest_main)
add_test( runUnitTests runUnitTests )
