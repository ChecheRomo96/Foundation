#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/common.sh"

usage() {
    printf '%s\n' "Usage: $0 <preset> [--parallel <jobs>] [--fresh] [--skip-export] [--package <prefix>]"
}

PRESET=""
PARALLEL=""
FRESH=0
SKIP_EXPORT=0
PACKAGE_PREFIX=""

while [ "$#" -gt 0 ]; do
    case "$1" in
        --parallel)
            foundation_require_value "$1" "${2:-}"
            PARALLEL=$2
            shift 2
            ;;
        --fresh)
            FRESH=1
            shift
            ;;
        --skip-export)
            SKIP_EXPORT=1
            shift
            ;;
        --package)
            foundation_require_value "$1" "${2:-}"
            PACKAGE_PREFIX=$2
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        -*)
            foundation_die "unknown option: $1"
            ;;
        *)
            [ -z "$PRESET" ] || foundation_die "only one preset may be specified"
            PRESET=$1
            shift
            ;;
    esac
done

foundation_require_preset "$PRESET"

if [ "$SKIP_EXPORT" -eq 0 ] && [ -n "$PACKAGE_PREFIX" ]; then
    foundation_die "--package requires --skip-export; normal validation exports to dist/$PRESET"
fi

if [ "$SKIP_EXPORT" -eq 0 ]; then
    set -- "$SCRIPT_DIR/export.sh" "$PRESET"
    [ "$FRESH" -eq 0 ] || set -- "$@" --fresh
    [ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
    "$@"
fi

[ -n "$PACKAGE_PREFIX" ] || PACKAGE_PREFIX="$FOUNDATION_DIST_ROOT/$PRESET"
PACKAGE_PREFIX=$(foundation_absolute_path "$PACKAGE_PREFIX")
[ -d "$PACKAGE_PREFIX" ] || foundation_die "package prefix not found: $PACKAGE_PREFIX"

foundation_require_configured "$PRESET"
FOUNDATION_BUILD_DIR=$(foundation_build_dir "$PRESET")
FOUNDATION_CACHE="$FOUNDATION_BUILD_DIR/CMakeCache.txt"
CONSUMER_SOURCE_DIR="$FOUNDATION_ROOT/tests/PackageConsumer"
CONSUMER_BUILD_DIR="$FOUNDATION_BUILD_ROOT/package-consumer/$PRESET"

foundation_cache_value() {
    sed -n "s/^$1:[^=]*=//p" "$FOUNDATION_CACHE" | sed -n '1p'
}

GENERATOR=$(foundation_cache_value CMAKE_GENERATOR)
GENERATOR_PLATFORM=$(foundation_cache_value CMAKE_GENERATOR_PLATFORM)
CXX_COMPILER=$(foundation_cache_value CMAKE_CXX_COMPILER)
TOOLCHAIN_FILE=$(foundation_cache_value CMAKE_TOOLCHAIN_FILE)
OSX_ARCHITECTURES=$(foundation_cache_value CMAKE_OSX_ARCHITECTURES)
CROSSCOMPILING=$(foundation_cache_value CMAKE_CROSSCOMPILING)

if [ "$CROSSCOMPILING" = "TRUE" ]; then
    foundation_die "package execution requires a native preset; validate cross-compiled packages in target firmware"
fi

cmake -E remove_directory "$CONSUMER_BUILD_DIR"

set -- cmake \
    -S "$CONSUMER_SOURCE_DIR" \
    -B "$CONSUMER_BUILD_DIR" \
    -G "$GENERATOR" \
    "-DFOUNDATION_PACKAGE_PREFIX=$PACKAGE_PREFIX"

[ -z "$GENERATOR_PLATFORM" ] || set -- "$@" -A "$GENERATOR_PLATFORM"
[ -z "$OSX_ARCHITECTURES" ] || set -- "$@" "-DCMAKE_OSX_ARCHITECTURES=$OSX_ARCHITECTURES"

if [ -n "$TOOLCHAIN_FILE" ]; then
    set -- "$@" "-DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE"
else
    case "$GENERATOR" in
        "Visual Studio"*|Xcode)
            ;;
        *)
            [ -z "$CXX_COMPILER" ] || \
                set -- "$@" "-DCMAKE_CXX_COMPILER=$CXX_COMPILER"
            ;;
    esac
fi

"$@"

set -- cmake --build "$CONSUMER_BUILD_DIR" --config Release
[ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
"$@"

set -- ctest \
    --test-dir "$CONSUMER_BUILD_DIR" \
    --build-config Release \
    --output-on-failure \
    --no-tests=error
"$@"

printf '%s\n' "Validated exported Foundation package at $PACKAGE_PREFIX"
