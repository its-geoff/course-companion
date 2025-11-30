# Course Companion

Homework tracker that allows students to track homework assignments, estimate grades, and organize their class schedules.

Current Version: 0.1.0 (alpha)

## How to Build with Conan and CMake

1.  navigate to the upper build folder with `cd ./build`
2.  download libraries with `conan install .. --build=missing -of build`
3.  generate build files with `cmake -S .. -B build -DCMAKE_TOOLCHAIN_FILE=build/build/conan_toolchain.cmake`
4.  navigate to the lower build folder with `cd build` and build the project with `cmake --build .`

## Deleting and Refreshing Builds

1.  navigate to the upper build folder with `cd ../build`
2.  remove the previous build with `rm -rf build/*`
3.  follow steps 2-4 of "How to Build with Conan and CMake"

## Testing Commands

-   to run all tests, use `ctest`
-   to run a certain test suite, use `ctest -R file_name`
