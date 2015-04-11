cmake_minimum_required(VERSION 2.8)

file(GLOB SRC *.cpp *.h)

message(STATUS ${RENDERER})

if(${RENDERER} STREQUAL D3D10_RENDER)
    add_definitions(-DD3D10_RENDER)
    set(DXSDK_LIBS  d3d10.lib
                    d3dx10.lib
                    d3dcompiler.lib
                    dxguid.lib
                    dinput8.lib
                    dxgi.lib)
                    
    file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/APIs/D3D10/*.cpp)
    source_group("Source Files\\D3D10" FILES ${LS})
    list(APPEND SRC ${LS})

    file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/APIs/D3D10/*.h)
    source_group("Header Files\\D3D10" FILES ${LS})
    list(APPEND SRC ${LS})
endif()

if(${RENDERER} STREQUAL D3D11_RENDER)
    add_definitions(-DD3D11_RENDER)
    set(DXSDK_LIBS  d3d11.lib
                    d3dx11.lib
                    d3dcompiler.lib
                    dxguid.lib
                    dinput8.lib
                    dxgi.lib)
                    
    file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/APIs/D3D11/*.cpp)
    source_group("Source Files\\D3D11" FILES ${LS})
    list(APPEND SRC ${LS})

    file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/APIs/D3D11/*.h)
    source_group("Header Files\\D3D11" FILES ${LS})
    list(APPEND SRC ${LS})
endif()

file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/SceneGraph/*.cpp)
source_group("Source Files\\SceneGraph" FILES ${LS})
list(APPEND SRC ${LS})

file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/SceneGraph/*.h)
source_group("Header Files\\SceneGraph" FILES ${LS})
list(APPEND SRC ${LS})

file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/TextLib/*.cpp)
source_group("Source Files\\TextLib" FILES ${LS})
list(APPEND SRC ${LS})

file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/TextLib/*.h)
source_group("Header Files\\TextLib" FILES ${LS})
list(APPEND SRC ${LS})
    
include_directories($ENV{DXSDK_DIR}/include)
include_directories(../../3rdparty/tinyxml2)

if(${EX_PLATFORM} EQUAL 64)
    link_directories($ENV{DXSDK_DIR}/lib/x64)
else()
    link_directories($ENV{DXSDK_DIR}/lib/x86)
endif()

add_library(GraphicsEngine STATIC ${SRC})

if(NOT UNIX)
	target_link_libraries(GraphicsEngine ${DXSDK_LIBS})
endif()

target_link_libraries(GraphicsEngine tinyxml2)
add_dependencies(GraphicsEngine Utilities MathLib)
