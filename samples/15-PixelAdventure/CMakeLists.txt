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

# Packaging
string( TOUPPER ${TARGET_NAME} COMPONENT_NAME )
set( CPACK_ARCHIVE_${COMPONENT_NAME}_FILE_NAME "PixelAdventure-${PROJECT_VERSION}" CACHE STRING "" FORCE )

install( TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION .
    COMPONENT ${TARGET_NAME}
)

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../assets/Pixel\ Adventure
    DESTINATION assets
    COMPONENT ${TARGET_NAME}
)

install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/../assets/fonts/arial.ttf
    DESTINATION assets/fonts
    COMPONENT ${TARGET_NAME}
)

install( FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/8-bit-powerup.mp3
    ${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/jump.mp3
    ${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/death.wav
    ${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/wood_break_1.wav
    ${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/wood_break_2.wav
    ${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/wood_break_3.wav
    ${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/wood_break_4.wav
    ${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/wood_break_5.wav
    ${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/wood_break_6.wav
    "${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/Sweet Treats.ogg"
    ${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/Puzzles.ogg
    ${CMAKE_CURRENT_SOURCE_DIR}/../assets/sounds/Radiohead.ogg
    DESTINATION assets/sounds
    COMPONENT ${TARGET_NAME}
)
