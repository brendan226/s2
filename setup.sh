#!/usr/bin/env bash
# setup.sh — download s2 build dependencies
#
# Prerequisites: mingw-w64 (g++, make), Vulkan SDK
#   Vulkan SDK: https://vulkan.lunarg.com/sdk/home
#
# Usage:
#   ./setup.sh           Download GLFW + ImGui into lib/
#   ./setup.sh --check   Check dependency status
set -euo pipefail

LIB_DIR="lib"
mkdir -p "$LIB_DIR"

info()    { echo "  [s2] $*"; }
success() { echo "  [s2] ✓ $*"; }
warn()    { echo "  [s2] ! $*" >&2; }

# ── Check mode ───────────────────────────────────────────────────────
if [[ "${1:-}" == "--check" ]]; then
    echo ""
    echo "  s2 dependency check:"
    command -v g++ &>/dev/null           && success "g++ found"          || warn "g++ not found (need mingw-w64)"
    command -v make &>/dev/null          && success "make found"         || warn "make not found"
    [ -f "$LIB_DIR/glfw/lib/libglfw3.a" ] && success "GLFW"            || warn "GLFW missing (run ./setup.sh)"
    [ -f "$LIB_DIR/imgui/imgui.h" ]     && success "ImGui source"      || warn "ImGui missing (run ./setup.sh)"
    [ -d "$LIB_DIR/vulkan/Include" ]     && success "Vulkan SDK"        || warn "Vulkan SDK missing"
    echo ""
    exit 0
fi

# ── GLFW ─────────────────────────────────────────────────────────────
GLFW_VERSION="3.4"

if [ -f "$LIB_DIR/glfw/lib/libglfw3.a" ]; then
    success "GLFW already present"
else
    info "Downloading GLFW ${GLFW_VERSION}..."
    tmp=$(mktemp -d)
    curl -fsSL "https://github.com/glfw/glfw/releases/download/${GLFW_VERSION}/glfw-${GLFW_VERSION}.bin.WIN64.zip" \
        -o "$tmp/glfw.zip"
    unzip -q "$tmp/glfw.zip" -d "$tmp"
    mkdir -p "$LIB_DIR/glfw/include" "$LIB_DIR/glfw/lib"
    cp -r "$tmp/glfw-${GLFW_VERSION}.bin.WIN64/include/GLFW" "$LIB_DIR/glfw/include/"
    cp "$tmp/glfw-${GLFW_VERSION}.bin.WIN64/lib-mingw-w64/libglfw3.a" "$LIB_DIR/glfw/lib/"
    rm -rf "$tmp"
    success "GLFW ${GLFW_VERSION}"
fi

# ── Dear ImGui (docking branch) ─────────────────────────────────────
IMGUI_BRANCH="docking"

if [ -f "$LIB_DIR/imgui/imgui.h" ]; then
    success "Dear ImGui already present"
else
    info "Downloading Dear ImGui (${IMGUI_BRANCH} branch)..."
    tmp=$(mktemp -d)
    curl -fsSL "https://github.com/ocornut/imgui/archive/refs/heads/${IMGUI_BRANCH}.tar.gz" \
        | tar -xz -C "$tmp"
    mkdir -p "$LIB_DIR/imgui"
    cp "$tmp"/imgui-*/*.h "$tmp"/imgui-*/*.cpp "$LIB_DIR/imgui/"
    cp "$tmp"/imgui-*/backends/imgui_impl_glfw.h "$LIB_DIR/imgui/"
    cp "$tmp"/imgui-*/backends/imgui_impl_glfw.cpp "$LIB_DIR/imgui/"
    cp "$tmp"/imgui-*/backends/imgui_impl_vulkan.h "$LIB_DIR/imgui/"
    cp "$tmp"/imgui-*/backends/imgui_impl_vulkan.cpp "$LIB_DIR/imgui/"
    rm -rf "$tmp"
    success "Dear ImGui (${IMGUI_BRANCH})"
fi

# ── Vulkan SDK (manual) ──────────────────────────────────────────────
if [ -d "$LIB_DIR/vulkan/Include" ]; then
    success "Vulkan SDK linked"
elif [ -n "${VULKAN_SDK:-}" ] && [ -d "${VULKAN_SDK}/Include" ]; then
    info "Linking Vulkan SDK from ${VULKAN_SDK}..."
    mkdir -p "$LIB_DIR/vulkan"
    cp -r "${VULKAN_SDK}/Include" "$LIB_DIR/vulkan/Include"
    cp -r "${VULKAN_SDK}/Lib" "$LIB_DIR/vulkan/Lib"
    cp -r "${VULKAN_SDK}/Bin" "$LIB_DIR/vulkan/Bin"
    success "Vulkan SDK linked"
else
    # Try common install paths
    for sdk_dir in "C:/VulkanSDK"/*/; do
        if [ -d "${sdk_dir}Include" ]; then
            info "Found Vulkan SDK at ${sdk_dir}"
            mkdir -p "$LIB_DIR/vulkan"
            cp -r "${sdk_dir}Include" "$LIB_DIR/vulkan/Include"
            cp -r "${sdk_dir}Lib" "$LIB_DIR/vulkan/Lib"
            cp -r "${sdk_dir}Bin" "$LIB_DIR/vulkan/Bin"
            success "Vulkan SDK linked"
            break
        fi
    done

    if [ ! -d "$LIB_DIR/vulkan/Include" ]; then
        warn "Vulkan SDK not found."
        info "Install from: https://vulkan.lunarg.com/sdk/home"
        info "Then re-run ./setup.sh"
    fi
fi

# ── Summary ──────────────────────────────────────────────────────────
echo ""
echo "  ──────────────────────────"
[ -f "$LIB_DIR/glfw/lib/libglfw3.a" ] && success "GLFW"       || warn "GLFW"
[ -f "$LIB_DIR/imgui/imgui.h" ]       && success "ImGui"      || warn "ImGui"
[ -d "$LIB_DIR/vulkan/Include" ]       && success "Vulkan SDK" || warn "Vulkan SDK"
echo "  ──────────────────────────"

if [ -f "$LIB_DIR/glfw/lib/libglfw3.a" ] && [ -f "$LIB_DIR/imgui/imgui.h" ] && [ -d "$LIB_DIR/vulkan/Include" ]; then
    echo ""
    echo "  All deps ready. Build with: make libs && make"
else
    echo ""
    echo "  Fix warnings above, then re-run ./setup.sh"
fi
echo ""
