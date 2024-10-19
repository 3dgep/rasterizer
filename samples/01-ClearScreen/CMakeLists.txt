cmake_minimum_required( VERSION 3.30.0 )

set( TARGET_NAME 01-ClearScreen )

set( SRC_FILES
    main.cpp
)

set( INC_FILES

)

set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_executable( ${TARGET_NAME} ${ALL_FILES})

target_link_libraries( ${TARGET_NAME} 
    PUBLIC SDL3::SDL3
)

if(WIN32 AND TARGET SDL3-shared)
    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:${TARGET_NAME}>
        VERBATIM
    )
endif()