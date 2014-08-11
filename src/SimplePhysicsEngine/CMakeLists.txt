cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SRC *.cpp)
file(GLOB_RECURSE HDR *.h)

add_library(SimplePhysicsEngine STATIC ${SRC} ${HDR})
add_dependencies(SimplePhysicsEngine Utilities MathLib)
