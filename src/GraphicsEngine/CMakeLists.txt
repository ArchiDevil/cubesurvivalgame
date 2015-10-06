file(GLOB SRC *.cpp *.h)

if(${RENDERER} STREQUAL D3D11_RENDER)
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
    
include_directories(../../3rdparty/tinyxml2)

add_library(GraphicsEngine STATIC ${SRC})

if(NOT UNIX)
    target_link_libraries(GraphicsEngine ${DXSDK_LIBS})
endif()

target_link_libraries(GraphicsEngine tinyxml2 Utilities MathLib)
target_link_libraries(GraphicsEngine DirectXTex)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
