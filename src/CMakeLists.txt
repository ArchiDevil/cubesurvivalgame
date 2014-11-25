cmake_minimum_required(VERSION 2.6)

if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE -DD3D10_RENDER)
    include_directories($ENV{DXSDK_DIR}/include)
else()
	add_definitions(-std=c++0x)
endif()

add_subdirectory(GraphicsEngine)
add_subdirectory(GUI)
add_subdirectory(MathLib)
add_subdirectory(Utilities)
add_subdirectory(SimplePhysicsEngine)
add_subdirectory(game)

set_target_properties(GraphicsEngine 		PROPERTIES FOLDER "Libs")
set_target_properties(GUI 				    PROPERTIES FOLDER "Libs")
set_target_properties(MathLib 			    PROPERTIES FOLDER "Libs")
set_target_properties(Utilities 			PROPERTIES FOLDER "Libs")
set_target_properties(SimplePhysicsEngine   PROPERTIES FOLDER "Libs")
set_target_properties(Game 				    PROPERTIES FOLDER "Apps")

option(BUILD_TOOLS "Build engine tools" YES)

if(BUILD_TOOLS)
	add_subdirectory(Converter)
	add_subdirectory(MeshEditor)
	set_target_properties(Converter  PROPERTIES FOLDER "Apps")
	set_target_properties(MeshEditor PROPERTIES FOLDER "Apps")
endif()

if(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    add_custom_target(CopyResources
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/resources/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MinSizeRel/resources/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/resources/
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/RelWithDebInfo/resources/)
    add_dependencies(Game CopyResources)
    add_dependencies(MeshEditor CopyResources)
    set_target_properties(CopyResources PROPERTIES FOLDER "CMake")
endif()