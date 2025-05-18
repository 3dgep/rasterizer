cmake_minimum_required( VERSION 3.12...3.31 )

set( INC_FILES
	inc/aligned_unique_ptr.hpp
    inc/graphics/BlendMode.hpp
	inc/graphics/Buffer.hpp
	inc/graphics/Color.hpp
    inc/graphics/Enums.hpp
	inc/graphics/Image.hpp
    inc/graphics/Rasterizer.hpp
    inc/graphics/RenderTarget.hpp
    inc/graphics/Sprite.hpp
    inc/graphics/Vertex.hpp
	inc/graphics/Window.hpp
    inc/stb_easy_font.h
    inc/stb_image.h
    inc/stb_image_write.h
    inc/stb_rect_pack.h
    inc/stb_truetype.h
)

set( SRC_FILES
    src/BlendMode.cpp
    src/Color.cpp
    src/Image.cpp
    src/Rasterizer.cpp
	src/Window.cpp
    src/stb_image.cpp
    src/stb_image_write.cpp
    src/stb_truetype.cpp
)

set( ALL_FILES 
    ${SRC_FILES} 
    ${INC_FILES} 
    ../.clang-format
)

add_library( graphics STATIC ${ALL_FILES} )

target_compile_features( graphics PUBLIC cxx_std_20 )

target_include_directories( graphics
    PUBLIC inc
)

target_link_libraries( graphics
    PUBLIC math SDL3::SDL3
)
