cmake_minimum_required( VERSION 3.12...3.31 )

set( INC_FILES
	inc/aligned_unique_ptr.hpp
	inc/graphics/Buffer.hpp
	inc/graphics/Color.hpp
	inc/graphics/Image.hpp
	inc/graphics/Window.hpp
)

set( SRC_FILES
    src/Color.cpp
	src/Window.cpp
)

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
    PUBLIC math SDL3::SDL3
)
