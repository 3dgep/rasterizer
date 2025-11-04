cmake_minimum_required( VERSION 3.12...3.31 )

set( INC_FILES
	inc/aligned_unique_ptr.hpp
    inc/graphics/BlendMode.hpp
	inc/graphics/Buffer.hpp
	inc/graphics/Color.hpp
    inc/graphics/Enums.hpp
    inc/graphics/Font.hpp
	inc/graphics/Image.hpp
    inc/graphics/Rasterizer.hpp
    inc/graphics/ResourceManager.hpp
    inc/graphics/SpriteAnimation.hpp
    inc/graphics/Sprite.hpp
    inc/graphics/SpriteSheet.hpp
    inc/graphics/Text.hpp
    inc/graphics/TileMap.hpp
    inc/graphics/Vertex.hpp
	inc/graphics/Window.hpp
    inc/hash.hpp
    inc/stb_easy_font.h
    inc/stb_image.h
    inc/stb_image_write.h
    inc/stb_rect_pack.h
    inc/stb_truetype.h
)

set( SRC_FILES
    src/BlendMode.cpp
    src/Color.cpp
    src/Font.cpp
    src/Image.cpp
    src/Rasterizer.cpp
    src/ResourceManager.cpp
    src/Sprite.cpp
    src/SpriteAnimation.cpp
    src/SpriteSheet.cpp
    src/Text.cpp
    src/TileMap.cpp
	src/Window.cpp
    src/stb_image.cpp
    src/stb_image_write.cpp
    src/stb_truetype.cpp
)

set( IMGUI_INC_FILES 
    ../externals/imgui/imconfig.h
    ../externals/imgui/imgui.h
    ../externals/imgui/imgui_internal.h
    ../externals/imgui/imstb_rectpack.h
    ../externals/imgui/imstb_textedit.h
    ../externals/imgui/imstb_truetype.h
)

set( IMGUI_SRC_FILES 
    ../externals/imgui/imgui.cpp
    ../externals/imgui/imgui_demo.cpp
    ../externals/imgui/imgui_draw.cpp
    ../externals/imgui/imgui_tables.cpp
    ../externals/imgui/imgui_widgets.cpp
)

set( IMGUI_BACKEND_FILES
    ../externals/imgui/backends/imgui_impl_sdl3.h
    ../externals/imgui/backends/imgui_impl_sdl3.cpp
    ../externals/imgui/backends/imgui_impl_sdlrenderer3.h
    ../externals/imgui/backends/imgui_impl_sdlrenderer3.cpp
)

source_group( imgui FILES ${IMGUI_INC_FILES} ${IMGUI_SRC_FILES} )
source_group( imgui/backends FILES ${IMGUI_BACKEND_FILES} )

set( IMGUI_FILES 
    ${IMGUI_INC_FILES}
    ${IMGUI_SRC_FILES}
    ${IMGUI_BACKEND_FILES}
)

set( ALL_FILES 
    ${SRC_FILES} 
    ${INC_FILES} 
    ${IMGUI_FILES}
    ../.clang-format
)

add_library( graphics STATIC ${ALL_FILES} )
add_library( sr::graphics ALIAS graphics )

target_compile_features( graphics PUBLIC cxx_std_23 )
target_compile_options( graphics
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_include_directories( graphics
    PUBLIC inc
    PUBLIC ../externals/imgui ../externals/imgui/backends
)

target_link_libraries( graphics
    PUBLIC math SDL3_ttf::SDL3_ttf SDL3::SDL3
)
