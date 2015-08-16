file(GLOB SRC *.cpp *.h)

file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/Objects/*.cpp)
source_group("Source Files\\Objects" FILES ${LS})
list(APPEND SRC ${LS})

file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/Objects/*.h ${CMAKE_CURRENT_SOURCE_DIR}/Models/*.hpp)
source_group("Header Files\\Objects" FILES ${LS})
list(APPEND SRC ${LS})

file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/Models/*.cpp)
source_group("Source Files\\Models" FILES ${LS})
list(APPEND SRC ${LS})

file(GLOB_RECURSE LS ${CMAKE_CURRENT_SOURCE_DIR}/Models/*.h ${CMAKE_CURRENT_SOURCE_DIR}/Models/*.hpp)
source_group("Header Files\\Models" FILES ${LS})
list(APPEND SRC ${LS})

if({RENDERER} STREQUAL D3D10_RENDER)
    include_directories($ENV{DXSDK_DIR}/include)
endif()

add_library(GUI STATIC ${SRC})
add_dependencies(GUI GraphicsEngine MathLib Utilities)

add_subdirectory(tests)
