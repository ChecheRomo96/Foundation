#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/common.sh"

usage() {
    printf '%s\n' "Usage: $0 <preset> [--config <name>] [--prefix <path>]"
}

PRESET=""
CONFIGURATION=""
PREFIX=""

while [ "$#" -gt 0 ]; do
    case "$1" in
        --config)
            foundation_require_value "$1" "${2:-}"
            CONFIGURATION=$2
            shift 2
            ;;
        --prefix)
            foundation_require_value "$1" "${2:-}"
            PREFIX=$2
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
foundation_require_configured "$PRESET"

BUILD_DIR=$(foundation_build_dir "$PRESET")
CONFIGURATION=$(foundation_configuration "$PRESET" "$CONFIGURATION" "Release")
foundation_require_configuration "$CONFIGURATION"
[ -n "$PREFIX" ] || PREFIX="$FOUNDATION_DIST_ROOT/$PRESET"
PREFIX=$(foundation_absolute_path "$PREFIX")

set -- cmake --install "$BUILD_DIR" --prefix "$PREFIX"
set -- "$@" --config "$CONFIGURATION"

"$@"
