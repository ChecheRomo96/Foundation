#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/common.sh"

usage() {
    printf '%s\n' "Usage: $0 <preset> [--config <name>] [--target <name>] [--parallel <jobs>] [--clean-first] [--fresh] [--examples-on]"
}

PRESET=""
CONFIGURATION=""
TARGET=""
PARALLEL=""
CLEAN_FIRST=0
FRESH=0
EXAMPLES_ON=0

while [ "$#" -gt 0 ]; do
    case "$1" in
        --config)
            foundation_require_value "$1" "${2:-}"
            CONFIGURATION=$2
            shift 2
            ;;
        --target)
            foundation_require_value "$1" "${2:-}"
            TARGET=$2
            shift 2
            ;;
        --parallel)
            foundation_require_value "$1" "${2:-}"
            PARALLEL=$2
            shift 2
            ;;
        --clean-first)
            CLEAN_FIRST=1
            shift
            ;;
        --fresh)
            FRESH=1
            shift
            ;;
        --examples-on)
            EXAMPLES_ON=1
            shift
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

set -- "$SCRIPT_DIR/configure.sh" "$PRESET"
[ "$FRESH" -eq 0 ] || set -- "$@" --fresh
[ "$EXAMPLES_ON" -eq 0 ] || set -- "$@" -- -DFOUNDATION_EXAMPLES=ON
"$@"

BUILD_DIR=$(foundation_build_dir "$PRESET")
CONFIGURATION=$(foundation_configuration "$PRESET" "$CONFIGURATION" "Debug")
foundation_require_configuration "$CONFIGURATION"

set -- cmake --build "$BUILD_DIR"
set -- "$@" --config "$CONFIGURATION"
[ -z "$TARGET" ] || set -- "$@" --target "$TARGET"
[ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
[ "$CLEAN_FIRST" -eq 0 ] || set -- "$@" --clean-first

"$@"
