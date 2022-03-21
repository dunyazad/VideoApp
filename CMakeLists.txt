cmake_minimum_required(VERSION 3.14)
project(VideoApp C CXX)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/glfw)

add_definitions(-DGL_SILENCE_DEPRECATION)

if(APPLE)
	list(APPEND EXTRA_LIBS
		"-framework OpenGL"
	)
elseif(WIN32)
	list(APPEND EXTRA_LIBS
		#"-lglu32 -lopengl32"
		"glu32"
		"opengl32"
	)
	#set(CMAKE_EXE_LINKER_FLAGS "-std=gnu99 -static -static-libgcc -static-libstdc++ -mwindows")
else()
	list(APPEND EXTRA_LIBS
		"-lGL -lGLU -lX11"
	)
endif()

include_directories(
	./lib/FFmpeg/include
)

link_directories(
	./lib/FFmpeg/bin
)

link_libraries(
	avcodec
	avdevice
	avfilter
	avformat
	avutil
	postproc
	swresample
	swscale
)

list(APPEND SOURCES
	src/main.cpp
	src/video_reader.cpp
)

add_executable(VideoApp ${SOURCES})
target_link_libraries(VideoApp glfw ${EXTRA_LIBS})

