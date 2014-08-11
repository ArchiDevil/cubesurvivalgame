cmake_minimum_required(VERSION 2.6)

project(Converter)

file(GLOB_RECURSE SOURCE *.cpp)
file(GLOB_RECURSE HEADERS *.h)

add_executable(Converter ${SOURCE} ${HEADERS})
add_dependencies(Converter Utilities)
target_link_libraries(Converter Utilities)