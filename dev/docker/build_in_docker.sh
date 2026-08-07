#!/bin/bash
# build_in_docker.sh - Run build in Docker
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/../.."

echo "Building in Docker container..."

if [[ "$(docker images -q polygonal-primordials-dev:latest 2> /dev/null)" == "" ]]; then
    echo "Building Docker image..."
    docker build -f dev/docker/Dockerfile.dev -t polygonal-primordials-dev:latest .
fi

docker run --rm \
    -v "$(pwd):/workspace" \
    -w /workspace \
    polygonal-primordials-dev:latest \
    bash -c "./build_all.sh package"

echo "✓ Build complete in Docker"
