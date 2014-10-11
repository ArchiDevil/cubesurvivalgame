cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SRC *.cpp *.h)

if(MSVC)
    # define visual studio installation dir
    if(MSVC10)
        string(REGEX REPLACE "Common7\\\\Tools\\\\" "VC" VCINSTALLDIR $ENV{VS100COMNTOOLS})
    elseif(MSVC11)
        string(REGEX REPLACE "Common7\\\\Tools\\\\" "VC" VCINSTALLDIR $ENV{VS110COMNTOOLS})
    elseif(MSVC12)
        string(REGEX REPLACE "Common7\\\\Tools\\\\" "VC" VCINSTALLDIR $ENV{VS120COMNTOOLS})
    endif()
else(MSVC)
    message(ERROR "Unable to find visual studio installation path")
endif(MSVC)

include_directories(${VCINSTALLDIR}/UnitTest/include)
include_directories($ENV{DXSDK_DIR}/include)

link_directories(${VCINSTALLDIR}/UnitTest/lib)
link_directories($ENV{DXSDK_DIR}/lib/x86)

add_library(UnitTests SHARED ${SRC})
target_link_libraries(UnitTests GraphicsEngine GUI Utilities MathLib)

set_target_properties(UnitTests PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
set_target_properties(UnitTests PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
set_target_properties(UnitTests PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
set_target_properties(UnitTests PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")

# ################################
# # GTest
# ################################
# enable_testing()
# include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# ################################
# # Unit Tests
# ################################
# # Add test cpp file
# add_executable( runUnitTests testgtest.cpp )
# # Link test executable against gtest & gtest_main
# target_link_libraries(runUnitTests gtest_main)
# add_test( runUnitTests runUnitTests )
