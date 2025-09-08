cmake_minimum_required( VERSION 3.30.0 )

set( TARGET_NAME 14-Pong )

set( INC_FILES
    inc/Ball.hpp
    inc/Game.hpp
    inc/Menu.hpp
)

set( SRC_FILES
    main.cpp
    src/Ball.cpp
    src/Game.cpp
    src/Menu.cpp
)


set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_executable( ${TARGET_NAME} ${ALL_FILES} )

target_include_directories( ${TARGET_NAME}
    PUBLIC inc
)

target_link_libraries( ${TARGET_NAME} 
    PRIVATE sr::graphics sr::math 00-Common
)
