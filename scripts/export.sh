#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/common.sh"

usage() {
    printf '%s\n' "Usage: $0 <preset> [--config <name>]... [--output <path>] [--parallel <jobs>] [--fresh] [--keep] [--examples-on] [-- <cmake arguments>]"
}

PRESET=""
CONFIGURATIONS=""
OUTPUT=""
PARALLEL=""
FRESH=0
KEEP=0
EXAMPLES_ON=0
CUSTOM_OUTPUT=0

while [ "$#" -gt 0 ]; do
    case "$1" in
        --config)
            foundation_require_value "$1" "${2:-}"
            CONFIGURATIONS="${CONFIGURATIONS}${CONFIGURATIONS:+ }$2"
            shift 2
            ;;
        --output)
            foundation_require_value "$1" "${2:-}"
            OUTPUT=$2
            CUSTOM_OUTPUT=1
            shift 2
            ;;
        --parallel)
            foundation_require_value "$1" "${2:-}"
            PARALLEL=$2
            shift 2
            ;;
        --fresh)
            FRESH=1
            shift
            ;;
        --keep)
            KEEP=1
            shift
            ;;
        --examples-on)
            EXAMPLES_ON=1
            shift
            ;;
        --)
            shift
            break
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

if [ "$EXAMPLES_ON" -eq 1 ]; then
    set -- "$@" -DFOUNDATION_EXAMPLES=ON
fi

foundation_require_preset "$PRESET"
[ -n "$CONFIGURATIONS" ] || CONFIGURATIONS="Debug Release"
for CONFIGURATION in $CONFIGURATIONS; do
    foundation_require_configuration "$CONFIGURATION"
done
if [ "$EXAMPLES_ON" -eq 1 ]; then
    RELEASE_SELECTED=0
    for CONFIGURATION in $CONFIGURATIONS; do
        [ "$CONFIGURATION" != "Release" ] || RELEASE_SELECTED=1
    done
    [ "$RELEASE_SELECTED" -eq 1 ] || \
        foundation_die "--examples-on requires the Release configuration"
fi

[ -n "$OUTPUT" ] || OUTPUT="$FOUNDATION_DIST_ROOT/$PRESET"
OUTPUT=$(foundation_absolute_path "$OUTPUT")

if [ "$KEEP" -eq 0 ] && [ "$CUSTOM_OUTPUT" -eq 1 ]; then
    foundation_die "custom export paths require --keep; use clean.sh to remove them explicitly"
fi

if [ "$FRESH" -eq 1 ]; then
    "$SCRIPT_DIR/configure.sh" "$PRESET" --fresh -- "$@"
else
    "$SCRIPT_DIR/configure.sh" "$PRESET" -- "$@"
fi

BUILD_DIR=$(foundation_build_dir "$PRESET")
for CONFIGURATION in $CONFIGURATIONS; do
    BUILD_TARGET=Foundation
    if [ "$CONFIGURATION" = "Release" ]; then
        BUILD_TARGET=FoundationExportArtifacts
    fi
    set -- cmake --build "$BUILD_DIR" --config "$CONFIGURATION" --target "$BUILD_TARGET"
    [ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
    "$@"
done

if [ "$KEEP" -eq 0 ]; then
    foundation_require_safe_dist_child "$OUTPUT"
    cmake -E remove_directory "$OUTPUT"
fi

for CONFIGURATION in $CONFIGURATIONS; do
    "$SCRIPT_DIR/install.sh" "$PRESET" --config "$CONFIGURATION" --prefix "$OUTPUT"
done

printf '%s\n' "Exported Foundation ($CONFIGURATIONS) to $OUTPUT"
