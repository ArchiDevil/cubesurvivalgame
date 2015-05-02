cmake_minimum_required(VERSION 2.8)

project(Converter)

set(SOURCES
            cBlkConverter.cpp
            cBlkConverter.h
            cObjConverter.cpp
            cObjConverter.h
            cXConverter.cpp
            cXConverter.h
            IConverter.h
            LIMSaver.h
            misc.h
            main.cpp)

add_executable(Converter ${SOURCES})
target_link_libraries(Converter MathLib Utilities)
