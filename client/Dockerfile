# syntax=docker/dockerfile:1

ARG QT_VERSION=6.10.1
ARG PLATFORM=linux/amd64

FROM --platform=${PLATFORM} ubuntu:24.04@sha256:4fbb8e6a8395de5a7550b33509421a2bafbc0aab6c06ba2cef9ebffbc7092d90 AS builder

ARG QT_VERSION
ENV DEBIAN_FRONTEND=noninteractive
ENV QT_VERSION=${QT_VERSION}
ENV QT_DIR=/opt/qt/${QT_VERSION}/gcc_64

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    clang \
    git \
    python3 \
    python3-pip \
    uuid-dev \
    libgl-dev \
    libglib2.0-dev \
    libdbus-1-dev \
    libxkbcommon-dev \
    libegl-dev \
    libfontconfig1-dev \
    libfreetype-dev \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install --break-system-packages conan aqtinstall

RUN aqt install-qt linux desktop ${QT_VERSION} -O /opt/qt

WORKDIR /app
COPY CMakeLists.txt conanfile.* ./
COPY src/ src/
COPY include/ include/
COPY tests/ tests/

RUN conan profile detect --force && \
    sed -i 's/compiler.cppstd=gnu17/compiler.cppstd=gnu20/' /root/.conan2/profiles/default

# test build
FROM builder AS test

RUN mkdir -p build/build_test && \
    conan install . --build=missing --output-folder=build/build_test -g CMakeDeps -g CMakeToolchain -s build_type=Debug

RUN cmake -S . -B build/build_test \
    -G Ninja \
    -DCMAKE_C_COMPILER=/usr/bin/clang \
    -DCMAKE_CXX_COMPILER=/usr/bin/clang++ \
    -DCMAKE_TOOLCHAIN_FILE=/app/build/build_test/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBUILD_TESTING=ON \
    -DCMAKE_PREFIX_PATH=${QT_DIR}

RUN cmake --build build/build_test --parallel $(nproc)

CMD ["ctest", "--test-dir", "/app/build/build_test", "--output-on-failure"]

# production build
FROM builder AS main

RUN mkdir -p build/build_main && \
    conan install . --build=missing --output-folder=build/build_main -g CMakeDeps -g CMakeToolchain -s build_type=Release

RUN cmake -S . -B build/build_main \
    -G Ninja \
    -DCMAKE_C_COMPILER=/usr/bin/clang \
    -DCMAKE_CXX_COMPILER=/usr/bin/clang++ \
    -DCMAKE_TOOLCHAIN_FILE=/app/build/build_main/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_TESTING=OFF \
    -DCMAKE_PREFIX_PATH=${QT_DIR}

RUN cmake --build build/build_main --target CourseCompanion --parallel $(nproc)

# release production build
FROM --platform=${PLATFORM} ubuntu:24.04@sha256:4fbb8e6a8395de5a7550b33509421a2bafbc0aab6c06ba2cef9ebffbc7092d90 AS production

ARG QT_VERSION
ENV DEBIAN_FRONTEND=noninteractive
ENV QT_DIR=/opt/qt/${QT_VERSION}/gcc_64

RUN apt-get update && apt-get install -y \
    libstdc++6 \
    libuuid1 \
    libgl1 \
    libglib2.0-0 \
    libdbus-1-3 \
    libxkbcommon0 \
    libegl1 \
    libfontconfig1 \
    libfreetype6 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=main ${QT_DIR}/lib ${QT_DIR}/lib
COPY --from=main ${QT_DIR}/plugins ${QT_DIR}/plugins
COPY --from=main /app/build/build_main/bin/CourseCompanion /app/

ENV LD_LIBRARY_PATH=${QT_DIR}/lib
CMD ["/app/CourseCompanion"]