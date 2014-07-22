cmake_minimum_required(VERSION 2.8) 

set(SRC math.cpp
		math.h
		math/intersections.cpp
		math/intersections.h
		math/AABB.cpp
		math/AABB.h
		math/OOBB.cpp
		math/OOBB.h
		math/matrix.h
		math/plane.h
		math/quaternion.h
		math/ray.h
		math/vectorsFuncs.h
		math/vector2.h
		math/vector3.h
		math/vector4.h)			# Установка переменной со списком исходников

if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
	add_definitions("/wd4005 /wd4996 /D_CRT_SECURE_NO_WARNINGS")
endif()

include_directories(math)

add_library(MathLib STATIC ${SRC})	# Создает либу с именем MathLib
