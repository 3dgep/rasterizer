cmake_minimum_required( VERSION 3.30.0 )

set( TARGET_NAME 02-Images )

set( SRC_FILES
    main.cpp
)

set( INC_FILES

)

set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_executable( ${TARGET_NAME} ${ALL_FILES})

target_link_libraries( ${TARGET_NAME} 
    PRIVATE 00-Common graphics math
)
