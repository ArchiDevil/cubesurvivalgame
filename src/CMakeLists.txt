cmake_minimum_required(VERSION 2.6)

if(MSVC)
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

if(MSVC AND WIN32)
add_subdirectory(GraphicsEngine)
    set_target_properties(GraphicsEngine PROPERTIES FOLDER "Libs")
    
    add_subdirectory(System)
    set_target_properties(System PROPERTIES FOLDER "Libs")
    
add_subdirectory(game)
    set_target_properties(Game PROPERTIES FOLDER "Apps")
endif()

add_subdirectory(SimplePhysicsEngine)
set_target_properties(SimplePhysicsEngine PROPERTIES FOLDER "Libs")

add_subdirectory(MathLib)
set_target_properties(MathLib PROPERTIES FOLDER "Libs")

add_subdirectory(Utilities)
set_target_properties(Utilities PROPERTIES FOLDER "Libs")

if(BUILD_TOOLS)
    add_subdirectory(Converter)
    set_target_properties(Converter  PROPERTIES FOLDER "Apps")
    
    if(MSVC AND WIN32)
    add_subdirectory(MeshEditor)
    set_target_properties(MeshEditor PROPERTIES FOLDER "Apps")
endif()
endif()

if(CMAKE_RUNTIME_OUTPUT_DIRECTORY AND MSVC AND WIN32)
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