cmake_minimum_required(VERSION 2.8)

project(LostIsland)

if(MSVC60 OR MSVC70 OR MSVC71 OR MSVC80 OR CMAKE_COMPILER_2005 OR MSVC90 OR MSVC10 OR MSVC11)
    message(FATAL_ERROR "Visual Studio below 2013 is not supported")
endif()

if(NOT MSVC)
    message(FATAL_ERROR "Only Visual Studio supported for now")
endif()

set(RootDir ${CMAKE_CURRENT_LIST_DIR}/src)
set(RENDERER D3D11_RENDER)

option(BUILD_TESTS "Generate projects for tests building" ON)

set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER CMake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# The NEW behavior for this policy is to ignore CMAKE_SHARED_LIBRARY_<Lang>_FLAGS whether it is modified or not and honor the POSITION_INDEPENDENT_CODE target property.
cmake_policy(SET CMP0018 NEW)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    message(STATUS "x64 detected")
    set(EX_PLATFORM 64)
else()
    message(STATUS "x86 detected")
    set(EX_PLATFORM 32)
endif()

add_subdirectory(3rdparty/tinyxml2)

set(JSONCPP_WITH_TESTS OFF CACHE BOOL "Run JSON with tests")
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "Run JSON post-build tests")
set(OLD_CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/jsoncpp")
add_subdirectory(3rdparty/jsoncpp)
set(CMAKE_SOURCE_DIR ${OLD_CMAKE_SOURCE_DIR})

set_property(TARGET jsoncpp_lib	PROPERTY FOLDER "3rdparty")

function(AddVSTest TestName Sources)
    if(NOT BUILD_TESTS OR NOT WIN32)
        return()
    endif()
    
    if(MSVC)
        # define visual studio installation dir
        if(MSVC12)
            string(REGEX REPLACE "Common7\\\\Tools\\\\" "VC" VCINSTALLDIR $ENV{VS120COMNTOOLS})
        elseif(MSVC14)
            string(REGEX REPLACE "Common7\\\\Tools\\\\" "VC" VCINSTALLDIR $ENV{VS140COMNTOOLS})
        endif()
    else(MSVC)
        message(ERROR "Unable to find visual studio installation path")
    endif(MSVC)

    include_directories(${VCINSTALLDIR}/UnitTest/include)
    include_directories($ENV{DXSDK_DIR}/include)

    link_directories(${VCINSTALLDIR}/UnitTest/lib)
    if(${EX_PLATFORM} EQUAL 64)
        link_directories($ENV{DXSDK_DIR}/lib/x64)
    else()
        link_directories($ENV{DXSDK_DIR}/lib/x86)
    endif()
    
    foreach(arg ${ARGN})
        list(APPEND Sources ${arg})
    endforeach(arg)

    add_library(${TestName} SHARED ${Sources})

    set_target_properties(${TestName} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
    set_target_properties(${TestName} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
    set_target_properties(${TestName} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    set_target_properties(${TestName} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
    set_property(TARGET ${TestName} PROPERTY FOLDER "Tests")
endfunction(AddVSTest)

function(VSTestLinkLibraries TestName LinkLibraries)
    if(NOT BUILD_TESTS)
        return()
    endif()
    foreach(arg ${ARGN})
        list(APPEND LinkLibraries ${arg})
    endforeach(arg)
    target_link_libraries(${TestName} ${LinkLibraries})
endfunction(VSTestLinkLibraries)

if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
    add_definitions("/wd4005 /wd4996 /wd4201 /wd4512 /wd4505 /D_CRT_SECURE_NO_WARNINGS /W4")
endif()

include_directories(${RootDir})
add_subdirectory(src)

if(BUILD_TESTS)
    #set(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
    #add_definitions("-D_VARIADIC_MAX=10")
    #add_subdirectory(3rdparty/gtest-1.7.0)
    #set_property(TARGET gtest PROPERTY FOLDER "Tests")
    #set_property(TARGET gtest_main PROPERTY FOLDER "Tests")
    #set_property(TARGET runUnitTests PROPERTY FOLDER "Tests")
endif()
