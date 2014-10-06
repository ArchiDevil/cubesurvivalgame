cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SRC *.cpp *.h)

include_directories($ENV{DXSDK_DIR}/include)

add_library(GUI STATIC ${SRC})	# Создает исполняемый файл с именем main
add_dependencies(GUI GraphicsEngine MathLib Utilities)