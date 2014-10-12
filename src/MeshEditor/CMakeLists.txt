cmake_minimum_required(VERSION 2.8)

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
    