#!/usr/bin/env bash
# setup.sh — download and configure s2 dependencies
#
# Usage: ./setup.sh
#
# Downloads GLFW and Dear ImGui into lib/. Vulkan SDK must be installed
# separately from https://vulkan.lunarg.com/sdk/home
set -euo pipefail

LIB_DIR="lib"
mkdir -p "$LIB_DIR"

info()    { echo "  [s2] $*"; }
success() { echo "  [s2] ✓ $*"; }
error()   { echo "  [s2] ✗ $*" >&2; exit 1; }

# ── GLFW ──────────────────────────────────────────────────────────────
GLFW_VERSION="3.4"
GLFW_URL="https://github.com/glfw/glfw/releases/download/${GLFW_VERSION}/glfw-${GLFW_VERSION}.bin.WIN64.zip"

if [ -d "$LIB_DIR/glfw" ]; then
    success "GLFW already present"
else
    info "Downloading GLFW ${GLFW_VERSION}..."
    tmp=$(mktemp -d)
    curl -fsSL "$GLFW_URL" -o "$tmp/glfw.zip"
    unzip -q "$tmp/glfw.zip" -d "$tmp"
    mkdir -p "$LIB_DIR/glfw/include" "$LIB_DIR/glfw/lib"
    cp -r "$tmp/glfw-${GLFW_VERSION}.bin.WIN64/include/GLFW" "$LIB_DIR/glfw/include/"
    cp "$tmp/glfw-${GLFW_VERSION}.bin.WIN64/lib-mingw-w64/libglfw3.a" "$LIB_DIR/glfw/lib/"
    rm -rf "$tmp"
    success "GLFW ${GLFW_VERSION} installed"
fi

# ── Dear ImGui ────────────────────────────────────────────────────────
IMGUI_VERSION="v1.91.8-docking"

if [ -d "$LIB_DIR/imgui" ] && [ -f "$LIB_DIR/imgui/imgui.h" ]; then
    success "Dear ImGui already present"
else
    info "Downloading Dear ImGui ${IMGUI_VERSION}..."
    tmp=$(mktemp -d)
    curl -fsSL "https://github.com/ocornut/imgui/archive/refs/tags/${IMGUI_VERSION}.tar.gz" | tar -xz -C "$tmp"
    mkdir -p "$LIB_DIR/imgui"
    cp "$tmp"/imgui-*/imgui*.h "$tmp"/imgui-*/imgui*.cpp "$LIB_DIR/imgui/"
    cp "$tmp"/imgui-*/backends/imgui_impl_glfw.h "$tmp"/imgui-*/backends/imgui_impl_glfw.cpp "$LIB_DIR/imgui/"
    cp "$tmp"/imgui-*/backends/imgui_impl_vulkan.h "$tmp"/imgui-*/backends/imgui_impl_vulkan.cpp "$LIB_DIR/imgui/"
    rm -rf "$tmp"

    # Build imgui static lib if Makefile exists
    if [ -f "$LIB_DIR/imgui/Makefile" ]; then
        info "Building imgui..."
        cd "$LIB_DIR/imgui" && make && cd ../..
    fi
    success "Dear ImGui ${IMGUI_VERSION} installed"
fi

# ── Vulkan SDK ────────────────────────────────────────────────────────
if [ -d "$LIB_DIR/vulkan" ]; then
    success "Vulkan SDK headers present in lib/vulkan"
elif [ -n "${VULKAN_SDK:-}" ]; then
    info "Linking Vulkan SDK from $VULKAN_SDK..."
    mkdir -p "$LIB_DIR/vulkan"
    ln -sfn "$VULKAN_SDK/Include" "$LIB_DIR/vulkan/Include"
    ln -sfn "$VULKAN_SDK/Lib" "$LIB_DIR/vulkan/Lib"
    success "Vulkan SDK linked"
else
    echo ""
    info "Vulkan SDK not found."
    info "Install from: https://vulkan.lunarg.com/sdk/home"
    info "Then either:"
    info "  1. Set VULKAN_SDK env var and re-run this script"
    info "  2. Copy Include/ and Lib/ to lib/vulkan/ manually"
    echo ""
fi

# ── Summary ───────────────────────────────────────────────────────────
echo ""
echo "  Dependencies:"
[ -d "$LIB_DIR/glfw" ]   && success "GLFW"   || error "GLFW missing"
[ -d "$LIB_DIR/imgui" ]  && success "ImGui"  || error "ImGui missing"
[ -d "$LIB_DIR/vulkan" ] && success "Vulkan" || info "Vulkan — install SDK manually"
echo ""
echo "  Ready to build: make"
echo ""
