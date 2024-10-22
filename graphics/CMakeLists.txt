cmake_minimum_required( VERSION 3.30.0 )

set( INC_FILES
	inc/aligned_unique_ptr.hpp
	inc/graphics/Buffer.hpp
	inc/graphics/Color.hpp
	inc/graphics/Image.hpp
	inc/graphics/Window.hpp
	inc/graphics/WindowImpl.hpp
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

add_library( graphics STATIC ${ALL_FILES} )

set_target_properties( graphics
    PROPERTIES
        CXX_STANDARD 20
)

target_include_directories( graphics
    PUBLIC inc
)

target_link_libraries( graphics
    PRIVATE SDL3::SDL3
	PUBLIC math
)
