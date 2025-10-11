set( SDL_TEST_LIBRARY OFF CACHE BOOL "Build the SDL3_test library" )
set( SDL_DISABLE_UNINSTALL  ON CACHE BOOL "Disable uninstallation of SDL3" )
set( INPUT_USE_SDL3 ON CACHE BOOL "Use SDL3 for input handling" )
set( AUDIO_BUILD_EXAMPLES OFF CACHE BOOL "Build samples for the Audio library" ) 

add_subdirectory( Audio )
add_subdirectory( SDL )
add_subdirectory( SDL_ttf )
add_subdirectory( glm )
add_subdirectory( input )

set( SDL_TARGETS
	SDL_uclibc
	SDL3-static
	SDL3-shared
	SDL3_ttf
	SDL3_ttf-static
	freetype
	harfbuzz
	plutosvg
	plutovg
)

set( GLM_TARGETS
	glm
	glm_shared
)

macro( set_targets_folder TARGETS FOLDER )
	foreach( target ${TARGETS} )
		if( TARGET ${target} )
			set_target_properties( ${target} PROPERTIES FOLDER ${FOLDER})
		endif()
	endforeach()
endmacro()

set_targets_folder( "${GLM_TARGETS}" "externals/glm" )
set_targets_folder( "${SDL_TARGETS}" "externals/SDL" )
set_targets_folder( "Audio;input_SDL3" "externals" )