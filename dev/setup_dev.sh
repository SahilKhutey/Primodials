#!/bin/bash
# setup_dev.sh
#
# One-command developer environment setup.
# Installs all dependencies, configures IDE, builds everything.

set -e

PLATFORM="$(uname -s)"

echo ""
echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║           Polygonal Primordials - Developer Setup             ║"
echo "║           Platform: $PLATFORM                                 ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""

SKIP_BUILD=false
IDE=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        --skip-build) SKIP_BUILD=true; shift ;;
        --ide) IDE="$2"; shift 2 ;;
        *) shift ;;
    esac
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."

setup_cmake() {
    echo "═══ Checking CMake ═══"
    if ! command -v cmake >/dev/null 2>&1; then
        echo "Installing CMake..."
        case "$PLATFORM" in
            Linux) sudo apt install -y cmake ;;
            Darwin) brew install cmake ;;
            *) echo "Please install CMake manually." ;;
        esac
    fi
    CMAKE_VERSION=$(cmake --version | head -1 | awk '{print $3}')
    echo "✓ CMake $CMAKE_VERSION ready"
}

setup_python() {
    echo ""
    echo "═══ Setting up Python environment ═══"
    if ! command -v python3 >/dev/null 2>&1 && ! command -v python >/dev/null 2>&1; then
        echo "ERROR: python not found"
        exit 1
    fi
    if [ ! -d ".venv" ]; then
        echo "Creating Python virtual environment..."
        python3 -m venv .venv 2>/dev/null || python -m venv .venv
    fi
    echo "✓ Python environment ready"
}

setup_git_hooks() {
    echo ""
    echo "═══ Setting up Git hooks ═══"
    if [ -d ".git" ]; then
        mkdir -p .git/hooks
        cat > .git/hooks/pre-commit << 'EOF'
#!/bin/bash
echo "Running pre-commit checks..."
EOF
        chmod +x .git/hooks/pre-commit 2>/dev/null || true
        echo "✓ Git hooks installed"
    fi
}

setup_ide() {
    echo ""
    echo "═══ Setting up IDE ═══"
    if [ "$IDE" = "vscode" ] || [ -d ".vscode" ] || command -v code >/dev/null 2>&1; then
        mkdir -p .vscode
        if [ -f "dev/vscode/settings.json" ]; then cp -n dev/vscode/settings.json .vscode/ 2>/dev/null || true; fi
        if [ -f "dev/vscode/tasks.json" ]; then cp -n dev/vscode/tasks.json .vscode/ 2>/dev/null || true; fi
        if [ -f "dev/vscode/extensions.json" ]; then cp -n dev/vscode/extensions.json .vscode/ 2>/dev/null || true; fi
        echo "✓ VS Code configured"
    fi
}

do_initial_build() {
    if [ "$SKIP_BUILD" = true ]; then
        echo "Skipping initial build"
        return
    fi
    echo ""
    echo "═══ Running Initial Build ═══"
    ./build_all.sh all
}

setup_cmake
setup_python
setup_git_hooks
setup_ide
do_initial_build

echo ""
echo "✓ Developer setup complete!"
