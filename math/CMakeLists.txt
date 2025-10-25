cmake_minimum_required( VERSION 3.12...3.31 )

# Optional SSE4.1 SIMD optimizations
option(SR_ENABLE_SIMD_SSE4_1 "Enable SSE4.1 optimizations for math library" ON)


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
add_library( sr::math ALIAS math )

target_compile_features( math PUBLIC cxx_std_20 )

target_include_directories( math
    PUBLIC inc
)

target_link_libraries( math
    PUBLIC glm
)

if(SR_ENABLE_SIMD_SSE4_1)
    # Use generator expressions for compiler-specific flags
    target_compile_options(math PUBLIC
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-msse4.1>
        $<$<CXX_COMPILER_ID:Intel>:/QxSSE4.1>
        # MSVC x86 only supports up to /arch:SSE2, x64 has SSE4.1 by default
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<NOT:$<BOOL:${CMAKE_CL_64}>>>:/arch:SSE2>
    )
    message(STATUS "SR: SSE4.1 optimizations enabled for math library")
endif()
