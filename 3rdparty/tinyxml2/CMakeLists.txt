cmake_minimum_required(VERSION 2.8) 

set(SRC tinyxml2.cpp
		tinyxml2.h)

add_library(tinyxml2 STATIC ${SRC})

set_property(TARGET tinyxml2 PROPERTY FOLDER "3rdparty")