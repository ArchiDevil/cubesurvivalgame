cmake_minimum_required(VERSION 2.8)

set(OutDir ${CMAKE_CURRENT_BINARY_DIR})
set(ProjectDir ${CMAKE_CURRENT_LIST_DIR})
set(RtDir ${RootDir})

string(REGEX REPLACE "/" "\\\\" OutDir "${OutDir}")
string(REGEX REPLACE "/" "\\\\" ProjectDir "${ProjectDir}")
string(REGEX REPLACE "/" "\\\\" RtDir "${RtDir}")

file(GLOB_RECURSE SRC *.cpp *.h)

include_directories(../../3rdparty/jsoncpp/include)
include_directories($ENV{DXSDK_DIR}/include)
link_directories($ENV{DXSDK_DIR}/lib/x86)

add_executable(MeshEditor ${SRC})
target_link_libraries(MeshEditor GraphicsEngine GUI Utilities MathLib)

if(MSVC)
   set_target_properties(MeshEditor PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
   set_target_properties(MeshEditor PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
   set_target_properties(MeshEditor PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
   set_target_properties(MeshEditor PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
endif(MSVC)

file(MAKE_DIRECTORY ${OutDir}/resources)
add_custom_command(TARGET MeshEditor
                   POST_BUILD
                   COMMAND "xcopy" ARGS "/Y" "${ProjectDir}\\settings_release.ini" "${OutDir}"
                   COMMAND "xcopy" ARGS "/E" "/Y" "${RtDir}\\resources" "${OutDir}\\resources"
                   COMMAND "del" ARGS "${OutDir}\\settings.ini"
                   COMMAND "rename" ARGS "${OutDir}\\settings_release.ini" "settings.ini"
                   COMMENT "Running post-build")
                   