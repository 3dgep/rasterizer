cmake_minimum_required( VERSION 3.12...3.31 )

set( INC_FILES
	inc/input/Gamepad.hpp
	inc/input/GamepadStateTracker.hpp
	inc/input/Input.hpp
	inc/input/Keyboard.hpp
	inc/input/KeyboardStateTracker.hpp
	inc/input/Mouse.hpp
	inc/input/MouseStateTracker.hpp
)

set ( SRC_FILES
	src/Gamepad.cpp
	src/GamepadStateTracker.cpp
	src/Input.cpp
	src/Keyboard.cpp
	src/KeyboardStateTracker.cpp
	src/Mouse.cpp
	src/MouseStateTracker.cpp
)

set ( ALL_FILES
	${INC_FILES}
	${SRC_FILES}
)

add_library( input STATIC ${ALL_FILES} )
add_library( sr::input ALIAS input )

target_compile_features( input PUBLIC cxx_std_20 )


target_include_directories( input
    PUBLIC inc
)

target_link_libraries( input
    PUBLIC SDL3::SDL3
)


