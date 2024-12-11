add_subdirectory( 00-Common )
add_subdirectory( 01-ClearScreen )
add_subdirectory( 02-Images )

set_target_properties(
	00-Common
	01-ClearScreen
	02-Images
	PROPERTIES 
		FOLDER samples
		VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set the startup project.
set_directory_properties( PROPERTIES
	VS_STARTUP_PROJECT 02-Images
)
