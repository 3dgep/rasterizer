cmake_minimum_required( VERSION 3.12...4.1.2 )

set( TARGET_NAME 15-PixelAdventure )

set( SRC_FILES
    main.cpp
)

set( INC_FILES

)

set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_executable( ${TARGET_NAME} ${ALL_FILES} )

target_link_libraries( ${TARGET_NAME} 
    PRIVATE sr::graphics sr::math input::SDL3 00-Common
)
