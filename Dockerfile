# syntax=docker/dockerfile:1

FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    clang \
    git \
    python3 \
    python3-pip \
    uuid-dev \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install --break-system-packages conan

WORKDIR /app
COPY CMakeLists.txt conanfile.* ./
COPY src/ src/
COPY include/ include/
COPY tests/ tests/

RUN conan profile detect --force

# test build
FROM builder AS test

RUN mkdir -p build/build_test && \
    conan install . --build=missing --output-folder=build/build_test

RUN cmake -S . -B build/build_test \
    -G Ninja \
    -DCMAKE_C_COMPILER=/usr/bin/clang \
    -DCMAKE_CXX_COMPILER=/usr/bin/clang++ \
    -DCMAKE_TOOLCHAIN_FILE=/app/build/build_test/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBUILD_TESTING=ON

RUN cmake --build build/build_test --parallel $(nproc)

CMD ["ctest", "--test-dir", "/app/build/build_test", "--output-on-failure"]

# production build
FROM builder AS main

RUN mkdir -p build/build_main && \
    conan install . --build=missing --output-folder=build/build_main

RUN cmake -S . -B build/build_main \
    -G Ninja \
    -DCMAKE_C_COMPILER=/usr/bin/clang \
    -DCMAKE_CXX_COMPILER=/usr/bin/clang++ \
    -DCMAKE_TOOLCHAIN_FILE=/app/build/build_main/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_TESTING=OFF

RUN cmake --build build/build_main --parallel $(nproc)

# release production build
FROM ubuntu:24.04 AS production

RUN apt-get update && apt-get install -y \
    libstdc++6 \
    uuid \
    && rm -rf /var/lib/apt/lists/*

COPY --from=main /app/build/build_main/bin/CourseCompanion /app/

CMD ["/app/CourseCompanion"]