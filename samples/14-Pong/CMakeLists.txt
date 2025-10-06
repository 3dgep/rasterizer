cmake_minimum_required( VERSION 3.30.0 )

set( TARGET_NAME 14-Pong )

set( INC_FILES
    inc/Ball.hpp
    inc/Game.hpp
    inc/GameState.hpp
    inc/MenuState.hpp
    inc/Paddle.hpp
    inc/Scores.hpp
    inc/Sound.hpp
    inc/State.hpp
    inc/StateBase.hpp
)

set( SRC_FILES
    main.cpp
    src/Ball.cpp
    src/Game.cpp
    src/GameState.cpp
    src/MenuState.cpp
    src/Paddle.cpp
    src/Scores.cpp
    src/Sound.cpp
    src/StateBase.cpp
)


set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_executable( ${TARGET_NAME} ${ALL_FILES} )

target_include_directories( ${TARGET_NAME}
    PUBLIC inc
)

target_link_libraries( ${TARGET_NAME} 
    PRIVATE sr::graphics sr::math input::SDL3 Audio::Audio 00-Common
)
