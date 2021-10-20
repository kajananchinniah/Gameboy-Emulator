# Base image
ARG BASE_IMAGE=ubuntu:18.04
FROM ${BASE_IMAGE}

# Working directory
ARG WORKING_DIRECTORY=/GB_Emulator
WORKDIR ${WORKING_DIRECTORY}

# Install important things + SDL2 + Gtest
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    vim \
    libsdl2-* \
  && rm -rf /var/lib/apt/lists/*

# Install GTest
RUN apt-get update && apt-get install -y \
  libgtest-dev \
  && cd /usr/src/googletest \
  && cmake . \
  && cmake --build . --target install \
  && rm -rf /var/lib/apt/lists/*

# Copy everything
COPY . .

# Compile and build
RUN mkdir -p ${WORKING_DIRECTORY}/build \
  && cd ${WORKING_DIRECTORY}/build \
  && cmake .. \
  && make \
  && mv main ../ \
  && cd ..
