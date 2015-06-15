file(GLOB SRC *.cpp *.h *.hpp Win32/*.cpp Win32/*.h)

add_library(System STATIC ${SRC})
