#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/common.sh"

usage() {
    printf '%s\n' "Usage: $0 <preset> [--config <name>] [--parallel <jobs>] [--filter <regex>] [--fresh] [--allow-no-tests]"
}

PRESET=""
CONFIGURATION=""
PARALLEL=""
FILTER=""
FRESH=0
ALLOW_NO_TESTS=0

while [ "$#" -gt 0 ]; do
    case "$1" in
        --config)
            foundation_require_value "$1" "${2:-}"
            CONFIGURATION=$2
            shift 2
            ;;
        --parallel)
            foundation_require_value "$1" "${2:-}"
            PARALLEL=$2
            shift 2
            ;;
        --filter)
            foundation_require_value "$1" "${2:-}"
            FILTER=$2
            shift 2
            ;;
        --fresh)
            FRESH=1
            shift
            ;;
        --allow-no-tests)
            ALLOW_NO_TESTS=1
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

BUILD_DIR=$(foundation_build_dir "$PRESET")
CONFIGURATION=$(foundation_configuration "$PRESET" "$CONFIGURATION" "Debug")
foundation_require_configuration "$CONFIGURATION"

if [ "$FRESH" -eq 1 ]; then
    "$SCRIPT_DIR/configure.sh" "$PRESET" --fresh -- \
        -DFOUNDATION_TESTING=ON
else
    "$SCRIPT_DIR/configure.sh" "$PRESET" -- \
        -DFOUNDATION_TESTING=ON
fi

set -- cmake --build "$BUILD_DIR" --config "$CONFIGURATION"
[ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
"$@"

set -- ctest --test-dir "$BUILD_DIR" --output-on-failure
[ "$ALLOW_NO_TESTS" -eq 1 ] || set -- "$@" --no-tests=error
set -- "$@" --build-config "$CONFIGURATION"
[ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
[ -z "$FILTER" ] || set -- "$@" --tests-regex "$FILTER"
"$@"
