# Building and Testing

## Prerequisites

To build and run the tests, you need to apply a fix to the input submodule due to a case sensitivity issue.

### Fix for input submodule

The `externals/input/CMakeLists.txt` file has incorrect case for the include paths. Change:

```cmake
set(INC_FILES
    inc/input/ButtonState.hpp
    inc/input/Gamepad.hpp
    inc/input/Input.hpp
    inc/input/Keyboard.hpp
    inc/input/Mouse.hpp
)
```

To:

```cmake
set(INC_FILES
    inc/Input/ButtonState.hpp
    inc/Input/Gamepad.hpp
    inc/Input/Input.hpp
    inc/Input/Keyboard.hpp
    inc/Input/Mouse.hpp
)
```

This is required because the actual directory is `inc/Input` (with capital I), not `inc/input`.

## Building the Tests

```bash
mkdir -p out/build
cd out/build
cmake ../.. -DSR_BUILD_SAMPLES=OFF -DSR_BUILD_TESTS=ON -DSDLTTF_VENDORED=ON
cmake --build . --target ColorTests
```

## Running the Tests

```bash
# Run directly
./tests/ColorTests

# Or use CTest
ctest --output-on-failure
```

All 9 tests should pass successfully.
