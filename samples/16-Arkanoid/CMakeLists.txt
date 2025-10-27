cmake_minimum_required( VERSION 3.22.1 )

set( TARGET_NAME 16-Arkanoid )

set( INC_FILES
    inc/Ball.hpp
    inc/Brick.hpp
    inc/Button.hpp
    inc/Bullet.hpp
    inc/Field.hpp
    inc/Game.hpp
    inc/GameOverState.hpp
    inc/HighScores.hpp
    inc/HighScoreState.hpp
    inc/IntroState.hpp
    inc/Level.hpp
    inc/ParseRects.hpp
    inc/PowerUp.hpp
    inc/Physics.hpp
    inc/PlayState.hpp
    inc/State.hpp
    inc/TitleState.hpp
    inc/Vaus.hpp
)

set( SRC_FILES
    src/main.cpp
    src/Ball.cpp
    src/Brick.cpp
    src/Button.cpp
    src/Bullet.cpp
    src/Field.cpp
    src/Game.cpp
    src/GameOverState.cpp
    src/HighScores.cpp
    src/HighScoreState.cpp
    src/IntroState.cpp
    src/Level.cpp
    src/ParseRects.cpp
    src/PowerUp.cpp
    src/Stages.hpp
    src/Physics.cpp
    src/PlayState.cpp
    src/tinyxml2.h
    src/tinyxml2.cpp
    src/TitleState.cpp
    src/Vaus.cpp
)

set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_executable( ${TARGET_NAME} ${ALL_FILES} )

target_include_directories( ${TARGET_NAME} PRIVATE inc )

target_link_libraries( ${TARGET_NAME} 
    PRIVATE sr::graphics sr::math input::SDL3 00-Common Audio::Audio LDtkLoader::LDtkLoader
)
