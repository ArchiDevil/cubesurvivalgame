cmake_minimum_required(VERSION 2.6)

if(MSVC)
    add_definitions(-DUNICODE -D_UNICODE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099")
    if({RENDERER} STREQUAL D3D10_RENDER)
        include_directories($ENV{DXSDK_DIR}/include)
    endif()
else()
    add_definitions(-std=c++0x)
endif()

add_definitions(-DNOMINMAX)

if(${RENDERER} STREQUAL D3D10_RENDER)
    add_definitions(-DD3D10_RENDER)
elseif(${RENDERER} STREQUAL D3D11_RENDER)
    add_definitions(-DD3D11_RENDER)
endif()

add_subdirectory(GraphicsEngine)
add_subdirectory(MathLib)
add_subdirectory(Utilities)
add_subdirectory(SimplePhysicsEngine)
add_subdirectory(game)

set_target_properties(GraphicsEngine        PROPERTIES FOLDER "Libs")
set_target_properties(MathLib               PROPERTIES FOLDER "Libs")
set_target_properties(Utilities             PROPERTIES FOLDER "Libs")
set_target_properties(SimplePhysicsEngine   PROPERTIES FOLDER "Libs")
set_target_properties(Game 	                PROPERTIES FOLDER "Apps")

option(BUILD_TOOLS "Build engine tools" YES)

if(BUILD_TOOLS)
    add_subdirectory(Converter)
    add_subdirectory(MeshEditor)
    set_target_properties(Converter  PROPERTIES FOLDER "Apps")
    set_target_properties(MeshEditor PROPERTIES FOLDER "Apps")
endif()

if(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    add_custom_target(CopyResources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/resources/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources/ ${CMAKE_BINARY_DIR}/src/game/resources/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources/ ${CMAKE_BINARY_DIR}/src/MeshEditor/resources/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_LIST_DIR}/settings.ini ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_LIST_DIR}/settings.ini ${CMAKE_BINARY_DIR}/src/game/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_LIST_DIR}/settings.ini ${CMAKE_BINARY_DIR}/src/MeshEditor/
        )
    add_dependencies(Game CopyResources)
    add_dependencies(MeshEditor CopyResources)
    set_target_properties(CopyResources PROPERTIES FOLDER "CMake")
endif()