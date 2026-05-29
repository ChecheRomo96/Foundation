#!/bin/sh
set -e
PRESET="$1"
PREFIX="$2"
[ -z "$PREFIX" ] && PREFIX="dist/$PRESET"
cmake --preset "$PRESET"
cmake --build --preset "$PRESET"
cmake --install "build/$PRESET" --prefix "$PREFIX"
