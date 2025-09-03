cmake_minimum_required( VERSION 3.30.0 )

set( TARGET_NAME 07-TileMaps )

include(FetchContent)

FetchContent_Declare(
	LDtkLoader
	GIT_REPOSITORY https://github.com/Madour/LDtkLoader.git
	GIT_TAG		   1.5.3.1
	EXCLUDE_FROM_ALL
)

message( STATUS "Fetching LDtkLoader..." )
FetchContent_MakeAvailable(LDtkLoader)

set_target_properties(
	LDtkLoader
    PROPERTIES FOLDER externals
)

set( SRC_FILES
    main.cpp
)

set( INC_FILES

)

set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_executable( ${TARGET_NAME} ${ALL_FILES} )

target_link_libraries( ${TARGET_NAME} 
    PRIVATE sr::graphics sr::math 00-Common LDtkLoader
)
