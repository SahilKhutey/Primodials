#!/bin/bash
# Install build dependencies for Ubuntu/Debian
set -e

echo "Installing dependencies for Ubuntu/Debian..."
sudo apt update

sudo apt install -y \
    build-essential \
    cmake \
    cmake-gui \
    git \
    ninja-build \
    pkg-config \
    curl \
    wget \
    g++ \
    gcc \
    clang \
    lld \
    libsdl3-dev \
    libsdl3-ttf-dev \
    libsdl3-mixer-dev \
    libsdl3-image-dev \
    libfreetype6-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libvulkan-dev \
    python3 \
    python3-pip \
    python3-venv \
    python3-dev

echo "✓ All dependencies installed"
