cmake_minimum_required( VERSION 3.12...3.31 )

set( INC_FILES
    inc/math/AABB.hpp
    inc/math/bitmask_operators.hpp
    inc/math/Camera2D.hpp
    inc/math/Camera3D.hpp
    inc/math/Circle.hpp
    inc/math/Intrinsics.hpp
    inc/math/Line.hpp
    inc/math/Math.hpp
    inc/math/OutCodes.hpp
    inc/math/Plane.hpp
    inc/math/Rect.hpp
    inc/math/Space.hpp
    inc/math/Sphere.hpp
    inc/math/Transform2D.hpp
    inc/math/Viewport.hpp
)

set( SRC_FILES
    src/Camera2D.cpp
    src/Camera3D.cpp
    src/Math.cpp
    src/Transform2D.cpp
)

set( ALL_FILES 
    ${SRC_FILES} 
    ${INC_FILES} 
    ../.clang-format
)

add_library( math STATIC ${ALL_FILES} )

target_compile_features( math PUBLIC cxx_std_20 )

target_include_directories( math
    PUBLIC inc
)

target_link_libraries( math
    PUBLIC glm
)
