FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive

# 镜像只提供构建所需依赖，配置与工具链均在工程目录内。
RUN apt-get update && apt-get install -y --no-install-recommends \
        ca-certificates \
        openjdk-21-jdk-headless \
        cmake \
        ninja-build \
        git \
        curl \
        wget \
        unzip \
        zip \
        p7zip-full \
        python3 \
        python3-venv \
        python3-pip \
        libgl1 \
        libglib2.0-0t64 \
        libfontconfig1 \
        libfreetype6 \
        libdbus-1-3 \
        libxkbcommon0 \
        libx11-6 \
        libx11-xcb1 \
        libxcb1 \
        libxcb-cursor0 \
        libxcb-icccm4 \
        libxcb-image0 \
        libxcb-keysyms1 \
        libxcb-randr0 \
        libxcb-render-util0 \
        libxcb-shape0 \
        libxcb-sync1 \
        libxcb-xfixes0 \
        libxcb-xinerama0 \
        libxcb-xkb1 \
        libxkbcommon-x11-0 \
        libxrender1 \
        libxi6 \
        libsm6 \
        libice6 \
    && rm -rf /var/lib/apt/lists/*
