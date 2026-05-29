#!/bin/sh
set -e
PRESET="$1"
cmake --preset "$PRESET"
cmake --build --preset "$PRESET"
