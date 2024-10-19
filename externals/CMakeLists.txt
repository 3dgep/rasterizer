set( SDL_TEST_LIBRARY OFF CACHE BOOL "Build the SDL3_test library" )
set( SDL_DISABLE_UNINSTALL  ON CACHE BOOL "Disable uninstallation of SDL3" )

add_subdirectory( SDL )

set( SDL_TARGETS
	SDL_uclibc
	SDL3-static
	SDL3-shared
)

foreach( target ${SDL_TARGETS} )
	if( TARGET ${target} )
		set_target_properties( ${target} PROPERTIES FOLDER externals/SDL)
	endif()
endforeach()