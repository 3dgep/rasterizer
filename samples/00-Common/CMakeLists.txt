cmake_minimum_required( VERSION 3.12...3.33 )

set( TARGET_NAME 00-Common )

set( INC
	inc/Timer.hpp
)

set( SRC
	src/Timer.cpp
)

add_library( ${TARGET_NAME} STATIC ${INC} ${SRC} )

target_include_directories( ${TARGET_NAME}
	PUBLIC ./inc
)

target_link_libraries( ${TARGET_NAME}
	PUBLIC graphics
)


