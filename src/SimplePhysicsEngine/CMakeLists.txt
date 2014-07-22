cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SRC *.cpp)
file(GLOB_RECURSE HDR *.h)

if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
	add_definitions("/wd4005 /wd4996 /D_CRT_SECURE_NO_WARNINGS")
endif()

add_library(SimplePhysicsEngine STATIC ${SRC} ${HDR})
add_dependencies(SimplePhysicsEngine Utilities MathLib)
