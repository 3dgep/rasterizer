cmake_minimum_required( VERSION 3.30.0 )

set( TARGET_NAME 12-Audio )

set( SRC_FILES
    main.cpp
)

set( INC_FILES

)

set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_executable( ${TARGET_NAME} ${ALL_FILES} )

target_link_libraries( ${TARGET_NAME} 
    PRIVATE sr::graphics sr::math 00-Common Audio::Audio
)
