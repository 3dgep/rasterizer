cmake_minimum_required(VERSION 3.12...3.31)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
  GIT_SHALLOW TRUE
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Create test executable
add_executable(ColorTests
    ColorTests.cpp
)

# Link against Google Test and the graphics library
target_link_libraries(ColorTests
    PRIVATE
    gtest_main
    sr::graphics
)

# Set C++ standard
target_compile_features(ColorTests PRIVATE cxx_std_23)

# Discover and register tests with CTest
include(GoogleTest)
gtest_discover_tests(ColorTests)
