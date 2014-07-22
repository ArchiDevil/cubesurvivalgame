cmake_minimum_required(VERSION 2.8)

set(SRC GUI.h
		InputHandler.cpp
		InputHandler.h
		Listener.cpp
		Listener.h
		MainListener.cpp
		MainListener.h
		Skinner.cpp
		Skinner.h
		Objects/Base.cpp
		Objects/Base.h
		Objects/Button.cpp
		Objects/Button.h
		Objects/Canvas.cpp
		Objects/Canvas.h
		Objects/GUIMisc.h
		Objects/Image.cpp
		Objects/Image.h
		Objects/List.cpp
		Objects/List.h
		Objects/Table.cpp
		Objects/Table.h
		Objects/Text.h
		Objects/Textbox.cpp
		Objects/Textbox.h
		Objects/Window.h)
				
if(MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
	add_definitions("/wd4005 /wd4996 /D_CRT_SECURE_NO_WARNINGS")
endif()

include_directories($ENV{DXSDK_DIR}/include)

add_library(GUI STATIC ${SRC})	# Создает исполняемый файл с именем main
add_dependencies(GUI GraphicsEngine MathLib Utilities)