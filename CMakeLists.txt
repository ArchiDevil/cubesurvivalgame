cmake_minimum_required(VERSION 2.8)

project(LostIsland)

set(RootDir ${CMAKE_CURRENT_LIST_DIR}/src)

set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER CMake)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

cmake_policy(SET CMP0018 NEW)

add_subdirectory(3rdparty/tinyxml2)
add_subdirectory(src)