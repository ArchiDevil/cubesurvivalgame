cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SRC *.cpp)
file(GLOB_RECURSE HDR *.h)

add_library(MathLib STATIC ${SRC} ${HDR})	# Создает либу с именем MathLib
