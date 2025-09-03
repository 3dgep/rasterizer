cmake_minimum_required( VERSION 3.12...3.33 )

configure_file( resource.rc.in "${CMAKE_CURRENT_BINARY_DIR}/resource.rc" @ONLY )

set( TARGET_NAME 00-Common )

set( INC
	inc/Timer.hpp
)

set( SRC
	src/Timer.cpp
)

add_library( ${TARGET_NAME} STATIC ${INC} ${SRC} )

if(WIN32)
    target_sources( ${TARGET_NAME} INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/resource.rc" )
endif()

target_include_directories( ${TARGET_NAME}
	PUBLIC ./inc
)

target_link_libraries( ${TARGET_NAME}
	PUBLIC sr::graphics
)


