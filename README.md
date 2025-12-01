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
    -   if testing, use `setup-deps -t` to output to `build_test`
3.  generate build files with `build-configure`
    -   if testing, use `build-configure -t` to turn tests on
4.  navigate to the lower build folder with `cd build` and build the project with `make-build`
    -   to build only main files, use `make-build -m`
    -   to build only test files, use `make-build -t`
    -   NOTE: for WSL, you will stay in the upper build folder

## Deleting and Refreshing Builds (do this after a failed build or after changes are made to code)

1.  navigate to the upper build folder with `cd ../build`
2.  remove the previous build with `rm-prev-build-___`
    -   use `build_main` or `build_test` based on build type
3.  follow steps 2-4 of "How to Build with Conan and CMake"

## Testing Commands

-   NOTE: make sure tests are built before using this
-   to run all tests, use `run-tests`
-   to run a certain test suite, use `ctest -R file_name`

## WSL Differences

-   path to root folder: `~/wsl_build/course-companion`
-   path to build folder: `~/wsl_build/course-companion/build/build`
