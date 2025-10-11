cmake_minimum_required( VERSION 3.12...4.1.2 )

set( TARGET_NAME 15-PixelAdventure )

set( INC_FILES
    inc/Background.hpp
    inc/Game.hpp
    inc/Level.hpp
)

set( SRC_FILES
    src/Background.cpp
    src/main.cpp
    src/Game.cpp
)

set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_executable( ${TARGET_NAME} ${ALL_FILES} )

target_include_directories( ${TARGET_NAME} PRIVATE inc )

target_link_libraries( ${TARGET_NAME} 
    PRIVATE sr::graphics sr::math input::SDL3 00-Common LDtkLoader::LDtkLoader
)
