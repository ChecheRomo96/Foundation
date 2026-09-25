#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/common.sh"

usage() {
    printf '%s\n' "Usage: $0 <preset> [--fresh] [-- <additional cmake configure arguments>]"
}

PRESET=""
FRESH=0

while [ "$#" -gt 0 ]; do
    case "$1" in
        --fresh)
            FRESH=1
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

foundation_require_preset "$PRESET"

if [ "$FRESH" -eq 1 ]; then
    cmake --preset "$PRESET" --fresh "$@"
else
    cmake --preset "$PRESET" "$@"
fi

