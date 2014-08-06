cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SRC *.cpp)
file(GLOB_RECURSE HDR *.h)

if(NOT WIN32)
    list(REMOVE_ITEM ${SRC} System/Application.cpp
							System/System.cpp
							cRegWorker.cpp)
	list(REMOVE_ITEM ${HDR} System/Application.h
							System/System.h
							cRegWorker.h)
endif()

if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
	add_definitions("/wd4005 /wd4996 /D_CRT_SECURE_NO_WARNINGS")
endif()

add_library(Utilities STATIC ${SRC} ${HDR})
#elseif (CMAKE_COMPILER_IS_GNUCXX)
