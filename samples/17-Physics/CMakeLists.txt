cmake_minimum_required( VERSION 3.30.0 )

set( TARGET_NAME 17-Physics )

set( INC_FILES
    Ball.hpp
)

set( SRC_FILES
    main.cpp
    Ball.cpp
)

set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_executable( ${TARGET_NAME} ${ALL_FILES} )

target_link_libraries( ${TARGET_NAME} 
    PRIVATE sr::graphics sr::math 00-Common
)
