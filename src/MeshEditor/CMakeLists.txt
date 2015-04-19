cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SRC *.cpp *.h)

include_directories($ENV{DXSDK_DIR}/include)

if(${EX_PLATFORM} EQUAL 64)
    link_directories($ENV{DXSDK_DIR}/lib/x64)
else()
    link_directories($ENV{DXSDK_DIR}/lib/x86)
endif()

add_executable(MeshEditor ${SRC})
target_link_libraries(MeshEditor GraphicsEngine Utilities MathLib)
target_link_libraries(MeshEditor freetype2 MyGUIDirectX11Platform MyGUIEngine)

if(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    foreach(arg ${CMAKE_CONFIGURATION_TYPES})
        file(COPY "../resources" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${arg})
        file(COPY "../saves" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${arg})
        file(COPY "../settings.ini" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${arg})
    endforeach(arg)
endif()

if(MSVC)
   set_target_properties(MeshEditor PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
   set_target_properties(MeshEditor PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
   set_target_properties(MeshEditor PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(MeshEditor PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(MSVC)
    