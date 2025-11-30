# Course Companion

Homework tracker that allows students to track homework assignments, estimate grades, and organize their class schedules.

Current Version: 0.1.0 (alpha)

## How to Set Up Scripts

1.  ensure all scripts are executable by using `chmod +x scripts/*` in the root directory
2.  add target directory (`/course-companion/scripts`) to PATH
3.  scripts should be ready to use by typing script name in correct directory

## How to Build with Conan and CMake

1.  navigate to the upper build folder with `cd ./build`
2.  download libraries with `setup-deps`
3.  generate build files with `build-configure`
4.  navigate to the lower build folder with `cd build` and build the project with `make-build`

## Deleting and Refreshing Builds

1.  navigate to the upper build folder with `cd ../build`
2.  remove the previous build with `rm-prev-build`
3.  follow steps 2-4 of "How to Build with Conan and CMake"

## Testing Commands

-   to run all tests, use `ctest`
-   to run a certain test suite, use `ctest -R file_name`
