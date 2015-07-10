file(GLOB SRC *.cpp *.h *.hpp System/*.cpp System/*.h)

if(NOT WIN32)
    list(REMOVE_ITEM SRC    System/Application.cpp
							System/System.cpp
							cRegWorker.cpp
                            System/Application.h
							System/System.h
							cRegWorker.h)
endif()

add_library(Utilities STATIC ${SRC})
add_subdirectory(tests)
