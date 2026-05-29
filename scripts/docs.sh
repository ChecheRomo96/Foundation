#!/bin/sh
set -e

PRESET="${1:-documentation}"

cmake --preset "$PRESET"
cmake --build --preset "$PRESET" --target docs
