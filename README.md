# Course Companion

Homework tracker that allows students to track homework assignments, estimate grades, and organize their class schedules.

Current Version: 0.1.0 (alpha)

## How to Build with Conan and CMake

-   navigate to the src folder with `cd ./src`
-   download libraries with `conan install .. --build=missing -of build`
-   navigate to the build folder with `cd ../build`
-   generate build files with `cmake .. -S .. -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake`
-   navigate to the build folder with `cd build` and build the project with `cmake --build .`
