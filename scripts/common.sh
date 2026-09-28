#!/bin/sh

. "$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)/romodular-adapter.sh"
. "$FOUNDATION_ROMODULAR_SCRIPTS/common.sh"

foundation_die() {
    romodular_die "$@"
}

foundation_require_command() {
    romodular_require_command "$@"
}

foundation_require_value() {
    romodular_require_value "$@"
}

foundation_require_preset() {
    romodular_require_preset "$@"
}

foundation_build_dir() {
    romodular_build_dir "$@"
}

foundation_configuration() {
    romodular_configuration "$@"
}

foundation_require_configuration() {
    romodular_require_configuration "$@"
}

foundation_require_configured() {
    romodular_require_configured "$@"
}

foundation_absolute_path() {
    romodular_absolute_path "$@"
}

foundation_require_safe_dist_child() {
    romodular_require_safe_dist_child "$@"
}
