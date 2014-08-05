cmake_minimum_required(VERSION 2.8)

project(LostIsland)

set(RootDir ${CMAKE_CURRENT_LIST_DIR}/src)

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

add_subdirectory(src)