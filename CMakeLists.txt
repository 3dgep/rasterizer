cmake_minimum_required( VERSION 3.30.0 )

option( BUILD_SHARED_LIBS "Build using shared libraries" OFF )
option( SR_BUILD_SAMPLES "Build samples." ON )

set( SR_VERSION_MAJOR 0 )
set( SR_VERSION_MINOR 0 )
set( SR_VERSION_PATCH 1 )
set( SR_VERSION ${SR_VERSION_MAJOR}.${SR_VERSION_MINOR}.${SR_VERSION_PATCH})

# Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project( SoftwareRasterizer VERSION ${SR_VERSION} LANGUAGES C CXX )


# Enable multi-processor builds in MSVC.
if(MSVC)
    add_compile_options(/MP)
endif(MSVC)

add_subdirectory( externals EXCLUDE_FROM_ALL )
add_subdirectory( graphics )
add_subdirectory( math )

if(SR_BUILD_SAMPLES)
    add_subdirectory(samples)
endif(SR_BUILD_SAMPLES)