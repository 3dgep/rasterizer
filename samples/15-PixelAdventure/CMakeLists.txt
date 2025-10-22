cmake_minimum_required( VERSION 3.12...4.1.2 )

set( TARGET_NAME 15-PixelAdventure )

set( INC_FILES
    inc/Background.hpp
    inc/Button.hpp
    inc/Character.hpp
    inc/Curve.hpp
    inc/Effect.hpp
    inc/Game.hpp
    inc/Level.hpp
    inc/Pickup.hpp
    inc/Player.hpp
    inc/Transition.hpp
)

set( SRC_FILES
    src/Background.cpp
    src/Box.cpp
    src/Button.cpp
    src/Character.cpp
    src/Effect.cpp
    src/Game.cpp
    src/Level.cpp
    src/main.cpp
    src/Pickup.cpp
    src/Player.cpp
    src/Transition.cpp
)

set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_executable( ${TARGET_NAME} ${ALL_FILES} )

target_include_directories( ${TARGET_NAME} PRIVATE inc )

target_link_libraries( ${TARGET_NAME} 
    PRIVATE sr::graphics sr::math input::SDL3 00-Common Audio::Audio LDtkLoader::LDtkLoader
)
