file(GLOB_RECURSE SRC math/*.cpp math/*.h math/*.hpp)
list(APPEND SRC math.h)

add_library(MathLib STATIC ${SRC})
add_subdirectory(tests)
