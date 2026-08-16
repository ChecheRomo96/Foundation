#!/bin/sh
PRESET="$1"
if [ -z "$PRESET" ]; then
  rm -rf build dist
else
  rm -rf "build/$PRESET" "dist/$PRESET"
fi
