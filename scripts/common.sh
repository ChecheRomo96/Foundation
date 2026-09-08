#!/bin/sh

FOUNDATION_SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
FOUNDATION_ROOT=$(CDPATH= cd -- "$FOUNDATION_SCRIPT_DIR/.." && pwd)
FOUNDATION_BUILD_ROOT="$FOUNDATION_ROOT/build"
FOUNDATION_DIST_ROOT="$FOUNDATION_ROOT/dist"

foundation_die() {
    printf '%s\n' "error: $*" >&2
    exit 2
}

foundation_require_command() {
    command -v "$1" >/dev/null 2>&1 || foundation_die "required command not found: $1"
}

foundation_require_value() {
    [ "$#" -ge 2 ] || foundation_die "internal error: foundation_require_value expects a flag and value"
    [ -n "$2" ] || foundation_die "$1 requires a value"
}

foundation_require_preset() {
    [ -n "${1:-}" ] || foundation_die "a CMake configure preset is required"
    case "$1" in
        *..*|*/*|*\\*)
            foundation_die "invalid preset name: $1"
            ;;
    esac
}

foundation_build_dir() {
    printf '%s\n' "$FOUNDATION_BUILD_ROOT/$1"
}

foundation_configuration() {
    if [ -n "${2:-}" ]; then
        printf '%s\n' "$2"
        return
    fi

    case "$1" in
        documentation)
            printf '%s\n' "Release"
            ;;
        *)
            printf '%s\n' "${3:-Debug}"
            ;;
    esac
}

foundation_require_configuration() {
    case "$1" in
        Debug|Release)
            ;;
        *)
            foundation_die "unsupported configuration '$1'; expected Debug or Release"
            ;;
    esac
}

foundation_require_configured() {
    BUILD_DIR=$(foundation_build_dir "$1")
    [ -f "$BUILD_DIR/CMakeCache.txt" ] || foundation_die "preset '$1' is not configured; run scripts/configure.sh $1 first"
}

foundation_absolute_path() {
    case "$1" in
        /*)
            printf '%s\n' "$1"
            ;;
        *)
            printf '%s\n' "$FOUNDATION_ROOT/$1"
            ;;
    esac
}

foundation_require_safe_dist_child() {
    case "$1" in
        "$FOUNDATION_DIST_ROOT"/*)
            ;;
        *)
            foundation_die "refusing to remove export path outside $FOUNDATION_DIST_ROOT: $1"
            ;;
    esac
}

foundation_require_command cmake
cd "$FOUNDATION_ROOT" || foundation_die "cannot enter repository root: $FOUNDATION_ROOT"
