cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SRC *.cpp)
file(GLOB_RECURSE HDR *.h)

set(DXSDK_LIBS  d3d10.lib
				d3dx10.lib
				d3dcompiler.lib
				dxguid.lib
				dinput8.lib
				dxgi.lib)

if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
	add_definitions("/wd4005 /wd4996 /D_CRT_SECURE_NO_WARNINGS")
endif()

include_directories($ENV{DXSDK_DIR}/include)
include_directories(../../3rdparty/tinyxml2)

link_directories($ENV{DXSDK_DIR}/lib/x86)

add_library(GraphicsEngine STATIC ${SRC} ${HDR})

if(NOT UNIX)
	target_link_libraries(GraphicsEngine ${DXSDK_LIBS})
endif()

target_link_libraries(GraphicsEngine tinyxml2)

add_dependencies(GraphicsEngine Utilities MathLib)
