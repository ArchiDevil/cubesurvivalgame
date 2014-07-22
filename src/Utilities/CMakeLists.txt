cmake_minimum_required(VERSION 2.8)

set(SRC dinput.cpp
		iniloader.cpp
		logger.cpp
		timer.cpp
		ut.cpp
		BlockStreamer.cpp)

if(WIN32)
    set(SRC ${SRC} System/Application.cpp
            System/System.cpp
            cRegWorker.cpp)
endif()

if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
	add_definitions("/wd4005 /wd4996 /D_CRT_SECURE_NO_WARNINGS")
endif()

add_library(Utilities STATIC ${SRC})
#elseif (CMAKE_COMPILER_IS_GNUCXX)
