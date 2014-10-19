file(GLOB SRC *.cpp *.h Objects/*.cpp Objects/*.h)

include_directories($ENV{DXSDK_DIR}/include)
add_library(GUI STATIC ${SRC})
add_dependencies(GUI GraphicsEngine MathLib Utilities)

add_subdirectory(tests)
