<a id="readme-top"></a>

<div align="center">
 
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![License][license-shield]][license-url]
[![CI Pipeline][ci-shield]][ci-url]
 
</div>
 
<br />
<div align="center">
  <a href="https://github.com/its-geoff/course-companion">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>
 
  <h3 align="center">Course Companion</h3>
 
  <p align="center">
    Your personal academic sidekick — track assignments, estimate grades, and organize your class schedule.
    <br />
    <a href="https://github.com/its-geoff/course-companion"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/its-geoff/course-companion">View Demo</a>
    &middot;
    <a href="https://github.com/its-geoff/course-companion/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    &middot;
    <a href="https://github.com/its-geoff/course-companion/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
</div>
 
---
 
<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#running-with-docker">Running with Docker</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#architecture">Architecture</a></li>
    <li><a href="#running-tests">Running Tests</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>
 
---
 
## About The Project
 
[![Product Screenshot][product-screenshot]](https://github.com/its-geoff/course-companion)
 
**Course Companion** started as a command-line homework tracker and is in the middle of growing into a full desktop app. It lets students manage their academic life through a structured hierarchy of terms, courses, and assignments, now backed by a persistent database and a Qt6 GUI that's actively replacing the original CLI as the primary interface.

I'm building this solo, in whatever hours I can find between other things, so consider this a living project rather than a finished product — features land in small, versioned increments and the README gets updated as they do.
 
Key features:
- Organize coursework into **terms** and **courses**
- Track **assignments** by category, due date, and completion status
- Enter grades as a **percentage** (`90.5`) or **points** (`17/20`) — Course Companion calculates the rest
- Automatic **weighted grade calculation** per course, with configurable grade weights and letter grade scale
- **GPA tracking** per term, weighted by credit hours
- **Persistent storage** so your terms, courses, and assignments survive between sessions
- A growing **Qt6 desktop GUI** (`MainWindow`, `TermView`, `CourseView`, `AssignmentView`) sitting alongside the original CLI
- Persistent CI/CD pipeline with automated testing and weekly promotion from `develop` to `main`
 
Current Version: **0.4.1 (alpha, in development)**
 
<p align="right">(<a href="#readme-top">back to top</a>)</p>
 
### Built With
 
* [![C++][cpp-shield]][cpp-url]
* [![Qt][qt-shield]][qt-url]
* [![CMake][cmake-shield]][cmake-url]
* [![Docker][docker-shield]][docker-url]
* [![GoogleTest][gtest-shield]][gtest-url]
* [![Conan][conan-shield]][conan-url]
 
<p align="right">(<a href="#readme-top">back to top</a>)</p>
 
---
 
## Getting Started
 
### Prerequisites
 
- [CMake](https://cmake.org/) 3.20+
- [Clang](https://clang.llvm.org/) (C++20)
- [Qt6](https://www.qt.io/) (for the GUI build)
- [Conan](https://conan.io/) package manager
- [Ninja](https://ninja-build.org/) build system
- `uuid-dev` and `libmysqlcppconn-dev` (Linux)
 
Install Conan via pip:
 
```sh
pip install conan
conan profile detect --force
```
 
### Installation
 
1. Clone the repository:
 
```sh
git clone https://github.com/its-geoff/course-companion.git
cd course-companion
```
 
2. Configure the build. This installs Conan dependencies and generates the CMake toolchain in one step:
 
```sh
cd build
./build-configure
```
 
To also enable test builds and coverage:
 
```sh
./build-configure -t -c
```
 
3. Build the project:
 
```sh
cd build_main
cmake --build .
```
 
4. Run the application:
 
```sh
./bin/CourseCompanion
```
 
### Running with Docker
 
The project includes a multi-stage Dockerfile. To build and run the production image:
 
```sh
docker build --target production -t course-companion .
docker run -it course-companion
```
 
To run the test suite inside Docker:
 
```sh
docker build --target test -t course-companion:test .
docker run --rm course-companion:test
```
 
<p align="right">(<a href="#readme-top">back to top</a>)</p>
 
---
 
## Usage
 
Course Companion is mid-migration from a CLI-only tool to a Qt6 desktop app, so both interfaces currently exist side by side while the GUI catches up to full feature parity.

Either way, the workflow follows the same hierarchy:
 
```
Term Menu  →  Course Menu  →  Assignment Menu
```
 
**Term Menu** — add, edit, select, or remove academic terms.
 
**Course Menu** — after selecting a term, manage the courses within it.
 
**Assignment Menu** — after selecting a course, manage assignments. From here you can:
- Add or remove assignments
- Enter grades as a percentage (`90.5`) or as points (`17/20`)
- View all, completed, or incomplete assignments
 
Grades are automatically aggregated up: assignment grades feed into course grades (weighted by category), and course grades feed into term GPA (weighted by credits). Data now persists to a database between runs, so nothing above needs to be re-entered every session.
 
<p align="right">(<a href="#readme-top">back to top</a>)</p>
 
---
 
## Architecture
 
The project follows an **MVC pattern**:
 
- **Model** (`src/model/`) — `Assignment`, `Course`, and `Term` classes hold data and grade calculation logic.
- **Controller** (`src/controller/`) — `AssignmentController`, `CourseController`, and `TermController` mediate between the views and models, maintaining title-to-ID mappings for case-insensitive lookups and propagating updates to whichever view is active.
- **View** (`src/view/`) — `CliView` handles the original terminal I/O. The Qt6 views (`MainWindow`, `TermView`, `CourseView`, `AssignmentView`, `FormDialog`) are being built out alongside it as the GUI takes over. Neither view layer contains application logic; all of that stays in the models and controllers.
 
Utility functions shared across the codebase live in `src/utils/utils.cpp`.
 
<p align="right">(<a href="#readme-top">back to top</a>)</p>
 
---
 
## Running Tests
 
Tests are built separately from the main executable using CMake's `BUILD_TESTING` flag.
 
Configure and build the test suite:
 
```sh
cd build
./build-configure -t
cd build_test
cmake --build .
```
 
Run all tests:
 
```sh
ctest --output-on-failure
```
 
Or use the helper script for verbose output:
 
```sh
./run-tests -V
```
 
To generate a code coverage report (requires Clang):
 
```sh
./run-tests -c
./generate-coverage-mac    # macOS
./generate-coverage-win    # Linux / Windows (WSL)
```
 
The test suite includes unit tests for all model, controller, and utility classes, as well as integration tests covering the full Term → Course → Assignment hierarchy.
 
<p align="right">(<a href="#readme-top">back to top</a>)</p>
 
---
 
## Roadmap
 
- [x] Persistent data storage (database integration)
- [x] Configurable grade weights per course
- [x] Configurable grade scale per course
- [ ] Full Qt6 GUI feature parity with the CLI
- [ ] GUI branding and visual polish
- [ ] Settings menu
- [ ] Cross-term cumulative GPA
 
See the [open issues](https://github.com/its-geoff/course-companion/issues) for a full list of proposed features and known issues.
 
<p align="right">(<a href="#readme-top">back to top</a>)</p>
 
---
 
## Contributing
 
Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.
 
If you have a suggestion, please fork the repo and create a pull request. You can also open an issue with the tag "enhancement". Don't forget to give the project a star!
 
1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request
 
<p align="right">(<a href="#readme-top">back to top</a>)</p>
 
---
 
## License
 
Distributed under the Apache License 2.0. See `LICENSE` for more information.
 
<p align="right">(<a href="#readme-top">back to top</a>)</p>
 
---
 
## Contact

Questions or feedback? Reach out via GitHub or open an issue on the repo.

Geoffrey Agustin, [its-geoff](https://github.com/its-geoff)

Project Link: [https://github.com/its-geoff/course-companion](https://github.com/its-geoff/course-companion)

<p align="right">(<a href="#readme-top">back to top</a>)</p>
 
---
 
<!-- MARKDOWN LINKS & IMAGES -->
[contributors-shield]: https://img.shields.io/github/contributors/its-geoff/course-companion.svg?style=for-the-badge
[contributors-url]: https://github.com/its-geoff/course-companion/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/its-geoff/course-companion.svg?style=for-the-badge
[forks-url]: https://github.com/its-geoff/course-companion/network/members
[stars-shield]: https://img.shields.io/github/stars/its-geoff/course-companion.svg?style=for-the-badge
[stars-url]: https://github.com/its-geoff/course-companion/stargazers
[issues-shield]: https://img.shields.io/github/issues/its-geoff/course-companion.svg?style=for-the-badge
[issues-url]: https://github.com/its-geoff/course-companion/issues
[license-shield]: https://img.shields.io/github/license/its-geoff/course-companion.svg?style=for-the-badge
[license-url]: https://github.com/its-geoff/course-companion/blob/main/LICENSE
[ci-shield]: https://img.shields.io/github/actions/workflow/status/its-geoff/course-companion/ci-cd.yaml?branch=develop&style=for-the-badge&label=CI%2FCD
[ci-url]: https://github.com/its-geoff/course-companion/actions/workflows/ci-cd.yaml
[product-screenshot]: images/screenshot.png
 
[cpp-shield]: https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=cplusplus&logoColor=white
[cpp-url]: https://en.cppreference.com/w/cpp/20
[qt-shield]: https://img.shields.io/badge/Qt-6-41CD52?style=for-the-badge&logo=qt&logoColor=white
[qt-url]: https://www.qt.io/
[cmake-shield]: https://img.shields.io/badge/CMake-3.20+-064F8C?style=for-the-badge&logo=cmake&logoColor=white
[cmake-url]: https://cmake.org/
[docker-shield]: https://img.shields.io/badge/Docker-2496ED?style=for-the-badge&logo=docker&logoColor=white
[docker-url]: https://www.docker.com/
[gtest-shield]: https://img.shields.io/badge/GoogleTest-1.14-4285F4?style=for-the-badge&logo=google&logoColor=white
[gtest-url]: https://github.com/google/googletest
[conan-shield]: https://img.shields.io/badge/Conan-2.x-6699CB?style=for-the-badge&logo=conan&logoColor=white
[conan-url]: https://conan.io/