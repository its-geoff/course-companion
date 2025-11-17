# Course Companion

Homework tracker that allows students to track homework assignments, estimate grades, and organize their class schedules.

Current Version: 0.1.0 (alpha)

## How to Build with Conan and CMake

-   download libraries with `conan install . --build=missing -of build`
-   generate build files with `cmake .. -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"`
-   build the project with `cmake --build .`
