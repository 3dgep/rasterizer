cmake_minimum_required( VERSION 3.30.0 )

set( INC_FILES
	inc/Graphics/Color.hpp
	inc/Graphics/Image.hpp
	inc/Graphics/Window.hpp
	inc/Graphics/WindowImpl.hpp
)

set( SRC_FILES
	src/Window.cpp
	
)

if( TARGET SDL3::SDL3 )
	list( APPEND SRC_FILES
		src/SDL3/WindowSDL3.hpp
		src/SDL3/WindowSDL3.cpp
	)
endif()

set( ALL_FILES 
    ${SRC_FILES} 
    ${INC_FILES} 
    ../.clang-format
)

add_library( Graphics STATIC ${ALL_FILES} )

set_target_properties( Graphics
    PROPERTIES
        CXX_STANDARD 20
)

target_include_directories( Graphics
    PUBLIC inc
)

target_link_libraries( Graphics
    PRIVATE SDL3::SDL3
)
