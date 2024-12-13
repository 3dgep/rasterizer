function( add_sample DIR_NAME )
	add_subdirectory( ${DIR_NAME} )
	set_target_properties( ${DIR_NAME}
		PROPERTIES 
			FOLDER samples
			VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
endfunction()

add_sample( 00-Common )
add_sample( 01-ClearScreen )
add_sample( 02-Images )
add_sample( 03-Triangle )

# Set the startup project.
set_directory_properties( PROPERTIES
	VS_STARTUP_PROJECT 02-Images
)
