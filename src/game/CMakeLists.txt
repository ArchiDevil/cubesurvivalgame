cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SRC *.cpp)
file(GLOB_RECURSE HDR *.h)

if(MSVC)
	add_definitions("/wd4005 /wd4996 /D_CRT_SECURE_NO_WARNINGS")
endif()

include_directories($ENV{DXSDK_DIR}/include)
link_directories($ENV{DXSDK_DIR}/lib/x86)

add_executable(Game ${SRC} ${HDR})
add_dependencies(Game GraphicsEngine GUI Utilities MathLib SimplePhysicsEngine)
target_link_libraries(Game GraphicsEngine GUI Utilities MathLib SimplePhysicsEngine)

set(OutDir ${CMAKE_CURRENT_BINARY_DIR})
set(ProjectDir ${CMAKE_CURRENT_LIST_DIR})
set(RtDir ${RootDir})

string(REGEX REPLACE "/" "\\\\" OutDir "${OutDir}")
string(REGEX REPLACE "/" "\\\\" ProjectDir "${ProjectDir}")
string(REGEX REPLACE "/" "\\\\" RtDir "${RtDir}")

#message(${CMAKE_CURRENT_LIST_FILE})

file(MAKE_DIRECTORY ${OutDir}/resources)
add_custom_command(TARGET Game
                   POST_BUILD
                   COMMAND "xcopy" ARGS "/Y" "${ProjectDir}\\settings_release.ini" "${OutDir}"
                   COMMAND "xcopy" ARGS "/E" "/Y" "${RtDir}\\resources" "${OutDir}\\resources"
                   COMMAND "del" ARGS "${OutDir}\\settings.ini"
                   COMMAND "rename" ARGS "${OutDir}\\settings_release.ini" "settings.ini"
                   COMMENT "Running post-build")