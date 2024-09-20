#!/bin/bash

set -e

SCRIPT_DIR=$(dirname "$(realpath "$0")")
ROOT_DIR=$(realpath "$SCRIPT_DIR/..")
SHADER_DIR="$ROOT_DIR/shaders"
OUT_DIR="$ROOT_DIR/resources/shaders"

mkdir -p "$OUT_DIR"

echo "Compiling shaders ..."

# shellcheck disable=SC2317
compile_shader() {
    local shader="$1"
    local shader_name
    shader_name=$(basename "$shader")
    local out_path="$OUT_DIR/$shader_name.spv"

    if glslangValidator -G "$shader" -o "$out_path"; then
        echo "Compiled: $shader → $out_path"
    else
        echo "Failed to compile: $shader_name" >&2
        exit 1
    fi
}

export -f compile_shader
export OUT_DIR

find "$SHADER_DIR" -type f -exec bash -c 'compile_shader "$0"' {} \;

echo "Done"
exit 0
