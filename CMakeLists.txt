cmake_minimum_required(VERSION 2.8)

project(LostIsland)

set(RootDir ${CMAKE_CURRENT_LIST_DIR}/src)
option(BUILD_TESTS "Generate projects for tests building" OFF)

set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER CMake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

cmake_policy(SET CMP0018 NEW)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(3rdparty/tinyxml2)

set(JSONCPP_WITH_TESTS OFF CACHE BOOL "Run JSON with tests")
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "Run JSON post-build tests")
set(OLD_CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/jsoncpp")
add_subdirectory(3rdparty/jsoncpp)
set(CMAKE_SOURCE_DIR ${OLD_CMAKE_SOURCE_DIR})

set_property(TARGET jsoncpp_lib	PROPERTY FOLDER "3rdparty")

if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
	add_definitions("/wd4005 /wd4996 /D_CRT_SECURE_NO_WARNINGS")
endif()

include_directories(src)
add_subdirectory(src)

if(BUILD_TESTS)
	set(gtest_force_shared_crt ON CACHE BOOL "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
	add_definitions("-D_VARIADIC_MAX=10")
	add_subdirectory(3rdparty/gtest-1.7.0)
	set_property(TARGET gtest PROPERTY FOLDER "Tests")
	set_property(TARGET gtest_main PROPERTY FOLDER "Tests")
	
	add_subdirectory(tests)
	set_property(TARGET UnitTests PROPERTY FOLDER "Tests")
	set_property(TARGET runUnitTests PROPERTY FOLDER "Tests")
endif()

