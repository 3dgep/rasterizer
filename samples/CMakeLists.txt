function( add_sample DIR_NAME )
	add_subdirectory( ${DIR_NAME} )
	set_target_properties( ${DIR_NAME}
		PROPERTIES 
			FOLDER samples
			VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
endfunction()

include(FetchContent)

FetchContent_Declare(
	LDtkLoader
	GIT_REPOSITORY https://github.com/Madour/LDtkLoader.git
	GIT_TAG		   1.5.3.1
	GIT_SHALLOW TRUE
	EXCLUDE_FROM_ALL
)

message( STATUS "Fetching LDtkLoader..." )
FetchContent_MakeAvailable(LDtkLoader)

set_target_properties(
	LDtkLoader
    PROPERTIES FOLDER externals
)

add_sample( 00-Common )
add_sample( 01-ClearScreen )
add_sample( 02-Images )
add_sample( 03-Triangle )
add_sample( 04-ImGui )
add_sample( 05-Sprites )
add_sample( 06-FillRules )
add_sample( 07-TileMaps )
add_sample( 08-Circles )
add_sample( 09-EdgeFunction )
add_sample( 10-Barycentric )
add_sample( 11-AddressModes )
add_sample( 12-Audio )
add_sample( 13-Input )
add_sample( 14-Pong )
add_sample( 15-PixelAdventure )
add_sample( 16-Arkanoid )
add_sample( 17-Physics )

# Set the startup project.
set_directory_properties( PROPERTIES
	VS_STARTUP_PROJECT 16-Arkanoid
)
