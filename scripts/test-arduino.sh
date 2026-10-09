#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/common.sh"

usage() {
    printf '%s\n' "Usage: $0 [--fqbn <board>] [--cpstl <source-directory>]"
    printf '%s\n' "CPSTL defaults to FOUNDATION_CPSTL_SOURCE or the sibling ../CPSTL."
}

# The declared Arduino source-mode board. Other cores are not validated here.
FQBN=arduino:avr:uno
CPSTL=${FOUNDATION_CPSTL_SOURCE:-$FOUNDATION_ROOT/../CPSTL}

while [ "$#" -gt 0 ]; do
    case "$1" in
        --fqbn)
            foundation_require_value "$1" "${2:-}"
            FQBN=$2
            shift 2
            ;;
        --cpstl)
            foundation_require_value "$1" "${2:-}"
            CPSTL=$2
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            foundation_die "unknown argument: $1"
            ;;
    esac
done

command -v arduino-cli >/dev/null 2>&1 || foundation_die "arduino-cli not found"
[ -f "$CPSTL/library.properties" ] ||
    foundation_die "CPSTL Arduino library not found at $CPSTL"
CPSTL=$(CDPATH= cd -- "$CPSTL" && pwd)

BUILD_ROOT="$FOUNDATION_ROOT/build/arduino/$(printf '%s' "$FQBN" | tr ':' '_')"
rm -rf "$BUILD_ROOT"

# Compile each sketch against the repository and CPSTL as libraries, exactly as
# an Arduino user who installed both would, with the core's unmodified flags
# (gnu++11 on Arduino AVR).
COUNT=0
for SKETCH in "$FOUNDATION_ROOT"/examples/*/*/*.ino; do
    SKETCH_DIR=$(dirname -- "$SKETCH")
    NAME=${SKETCH_DIR#"$FOUNDATION_ROOT/examples/"}
    LOG="$BUILD_ROOT/$NAME.log"
    mkdir -p -- "$(dirname -- "$LOG")"
    printf '%s\n' "== $NAME ($FQBN)"

    STATUS=0
    arduino-cli compile \
        --fqbn "$FQBN" \
        --library "$FOUNDATION_ROOT" \
        --library "$CPSTL" \
        --build-path "$BUILD_ROOT/$NAME" \
        --warnings default \
        "$SKETCH_DIR" >"$LOG" 2>&1 || STATUS=$?
    cat -- "$LOG"
    [ "$STATUS" -eq 0 ] || foundation_die "$NAME failed to compile"

    # The stock core passes -fpermissive, which demotes real type errors to
    # warnings; any warning in Foundation or its examples fails the gate.
    if grep -F "$FOUNDATION_ROOT/" "$LOG" | grep -q "warning:"; then
        foundation_die "$NAME compiled with Foundation warnings"
    fi
    COUNT=$((COUNT + 1))
done

[ "$COUNT" -gt 0 ] || foundation_die "no Arduino sketches found"
printf '%s\n' "All $COUNT Arduino sketches compiled for $FQBN."
