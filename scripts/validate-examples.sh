#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/common.sh"

usage() {
    printf '%s\n' "Usage: $0 <preset> [--parallel <jobs>] [--fresh]"
}

PRESET=""
PARALLEL=""
FRESH=0

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
foundation_require_command cmp
foundation_require_command find
foundation_require_command grep

BUILD_DIR=$(foundation_build_dir "$PRESET")
DIST_DIR="$FOUNDATION_DIST_ROOT/$PRESET"
DEBUG_DIR="$BUILD_DIR/bin/Debug"
RELEASE_DIR="$BUILD_DIR/bin/Release"
VALIDATION_DIR="$BUILD_DIR/example-validation"
EXPORTED_DIR="$DIST_DIR/bin/examples"

set -- "$SCRIPT_DIR/build.sh" "$PRESET" \
    --config Debug \
    --target FoundationExamples \
    --examples-on
[ "$FRESH" -eq 0 ] || set -- "$@" --fresh
[ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
"$@"

set -- "$SCRIPT_DIR/build.sh" "$PRESET" \
    --config Release \
    --target FoundationExamples \
    --examples-on
[ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
"$@"

EXPECTED_COUNT=$(find "$FOUNDATION_ROOT/examples" \
    -type f -path '*/Desktop/CMakeLists.txt' -print | wc -l | tr -d ' ')
[ "$EXPECTED_COUNT" -gt 0 ] || foundation_die "no desktop example definitions were found"
[ -d "$DEBUG_DIR" ] || foundation_die "Debug example directory not found: $DEBUG_DIR"
[ -d "$RELEASE_DIR" ] || foundation_die "Release example directory not found: $RELEASE_DIR"

cmake -E remove_directory "$VALIDATION_DIR"
cmake -E make_directory \
    "$VALIDATION_DIR/Debug" \
    "$VALIDATION_DIR/Release" \
    "$VALIDATION_DIR/Export" \
    "$VALIDATION_DIR/Launcher"

foundation_run_example() {
    FOUNDATION_EXAMPLE_EXECUTABLE=$1
    FOUNDATION_EXAMPLE_STDOUT=$2
    FOUNDATION_EXAMPLE_STDERR=$3

    if "$FOUNDATION_EXAMPLE_EXECUTABLE" \
        >"$FOUNDATION_EXAMPLE_STDOUT" \
        2>"$FOUNDATION_EXAMPLE_STDERR"; then
        return
    else
        FOUNDATION_EXAMPLE_STATUS=$?
    fi

    printf '%s\n' \
        "example failed with exit code $FOUNDATION_EXAMPLE_STATUS: $FOUNDATION_EXAMPLE_EXECUTABLE" >&2
    [ ! -s "$FOUNDATION_EXAMPLE_STDOUT" ] || cat "$FOUNDATION_EXAMPLE_STDOUT" >&2
    [ ! -s "$FOUNDATION_EXAMPLE_STDERR" ] || cat "$FOUNDATION_EXAMPLE_STDERR" >&2
    exit "$FOUNDATION_EXAMPLE_STATUS"
}

foundation_require_same_output() {
    FOUNDATION_EXPECTED_OUTPUT=$1
    FOUNDATION_ACTUAL_OUTPUT=$2
    FOUNDATION_OUTPUT_DESCRIPTION=$3

    cmp -s "$FOUNDATION_EXPECTED_OUTPUT" "$FOUNDATION_ACTUAL_OUTPUT" ||
        foundation_die "$FOUNDATION_OUTPUT_DESCRIPTION differs"
}

DEBUG_COUNT=0
for DEBUG_EXECUTABLE in "$DEBUG_DIR"/Foundation_*_d; do
    [ -f "$DEBUG_EXECUTABLE" ] || continue
    [ -x "$DEBUG_EXECUTABLE" ] || foundation_die "Debug example is not executable: $DEBUG_EXECUTABLE"
    DEBUG_COUNT=$((DEBUG_COUNT + 1))
done
[ "$DEBUG_COUNT" -eq "$EXPECTED_COUNT" ] || \
    foundation_die "expected $EXPECTED_COUNT Debug examples, found $DEBUG_COUNT"

RELEASE_COUNT=0
for RELEASE_EXECUTABLE in "$RELEASE_DIR"/Foundation_*; do
    [ -f "$RELEASE_EXECUTABLE" ] || continue
    [ -x "$RELEASE_EXECUTABLE" ] || foundation_die "Release example is not executable: $RELEASE_EXECUTABLE"

    RELEASE_NAME=${RELEASE_EXECUTABLE##*/}
    case "$RELEASE_NAME" in
        *_d)
            foundation_die "Debug-named executable found in Release directory: $RELEASE_NAME"
            ;;
    esac

    DEBUG_EXECUTABLE="$DEBUG_DIR/${RELEASE_NAME}_d"
    [ -x "$DEBUG_EXECUTABLE" ] || foundation_die "matching Debug example not found: $DEBUG_EXECUTABLE"

    foundation_run_example \
        "$DEBUG_EXECUTABLE" \
        "$VALIDATION_DIR/Debug/${RELEASE_NAME}.stdout.txt" \
        "$VALIDATION_DIR/Debug/${RELEASE_NAME}.stderr.txt"
    foundation_run_example \
        "$RELEASE_EXECUTABLE" \
        "$VALIDATION_DIR/Release/${RELEASE_NAME}.stdout.txt" \
        "$VALIDATION_DIR/Release/${RELEASE_NAME}.stderr.txt"

    foundation_require_same_output \
        "$VALIDATION_DIR/Debug/${RELEASE_NAME}.stdout.txt" \
        "$VALIDATION_DIR/Release/${RELEASE_NAME}.stdout.txt" \
        "$RELEASE_NAME Debug/Release stdout"
    foundation_require_same_output \
        "$VALIDATION_DIR/Debug/${RELEASE_NAME}.stderr.txt" \
        "$VALIDATION_DIR/Release/${RELEASE_NAME}.stderr.txt" \
        "$RELEASE_NAME Debug/Release stderr"

    RELEASE_COUNT=$((RELEASE_COUNT + 1))
done
[ "$RELEASE_COUNT" -eq "$EXPECTED_COUNT" ] || \
    foundation_die "expected $EXPECTED_COUNT Release examples, found $RELEASE_COUNT"

set -- "$SCRIPT_DIR/export.sh" "$PRESET" --examples-on
[ -z "$PARALLEL" ] || set -- "$@" --parallel "$PARALLEL"
"$@"
"$SCRIPT_DIR/validate-package.sh" "$PRESET"

[ -d "$EXPORTED_DIR" ] || foundation_die "exported example directory not found: $EXPORTED_DIR"

EXPORTED_COUNT=0
EXPORTED_FILE_COUNT=0
for EXPORTED_FILE in "$EXPORTED_DIR"/*; do
    [ -f "$EXPORTED_FILE" ] || continue
    EXPORTED_FILE_COUNT=$((EXPORTED_FILE_COUNT + 1))
    EXPORTED_NAME=${EXPORTED_FILE##*/}

    case "$EXPORTED_NAME" in
        FoundationExamples.command)
            [ "$(uname -s)" = "Darwin" ] || \
                foundation_die "macOS launcher exported on non-macOS host"
            ;;
        Foundation_*)
            case "$EXPORTED_NAME" in
                *_d|*_d.*)
                    foundation_die "Debug example was exported: $EXPORTED_NAME"
                    ;;
            esac
            [ -x "$EXPORTED_FILE" ] || foundation_die "exported example is not executable: $EXPORTED_FILE"
            EXPORTED_COUNT=$((EXPORTED_COUNT + 1))
            ;;
        *)
            foundation_die "unexpected file in exported example directory: $EXPORTED_NAME"
            ;;
    esac
done
[ "$EXPORTED_COUNT" -eq "$EXPECTED_COUNT" ] || \
    foundation_die "expected $EXPECTED_COUNT exported examples, found $EXPORTED_COUNT"

EXPECTED_FILE_COUNT=$EXPECTED_COUNT
case "$(uname -s)" in
    Darwin)
        EXPECTED_FILE_COUNT=$((EXPECTED_FILE_COUNT + 1))
        LAUNCHER="$EXPORTED_DIR/FoundationExamples.command"
        [ -x "$LAUNCHER" ] || foundation_die "macOS example launcher not found or not executable"
        ;;
    Linux)
        [ ! -e "$EXPORTED_DIR/FoundationExamples.command" ] || \
            foundation_die "macOS launcher must not be exported on Linux"
        [ ! -e "$EXPORTED_DIR/FoundationExamples.cmd" ] || \
            foundation_die "Windows launcher must not be exported on Linux"
        ;;
    *)
        foundation_die "validate-examples.sh supports native macOS and Linux hosts"
        ;;
esac
[ "$EXPORTED_FILE_COUNT" -eq "$EXPECTED_FILE_COUNT" ] || \
    foundation_die "expected $EXPECTED_FILE_COUNT files in exported example directory, found $EXPORTED_FILE_COUNT"

for RELEASE_EXECUTABLE in "$RELEASE_DIR"/Foundation_*; do
    [ -f "$RELEASE_EXECUTABLE" ] || continue
    RELEASE_NAME=${RELEASE_EXECUTABLE##*/}
    case "$RELEASE_NAME" in
        *_d)
            continue
            ;;
    esac

    EXPORTED_EXECUTABLE="$EXPORTED_DIR/$RELEASE_NAME"
    [ -x "$EXPORTED_EXECUTABLE" ] || foundation_die "exported example not found: $EXPORTED_EXECUTABLE"
    foundation_run_example \
        "$EXPORTED_EXECUTABLE" \
        "$VALIDATION_DIR/Export/${RELEASE_NAME}.stdout.txt" \
        "$VALIDATION_DIR/Export/${RELEASE_NAME}.stderr.txt"
    foundation_require_same_output \
        "$VALIDATION_DIR/Release/${RELEASE_NAME}.stdout.txt" \
        "$VALIDATION_DIR/Export/${RELEASE_NAME}.stdout.txt" \
        "$RELEASE_NAME build/export stdout"
    foundation_require_same_output \
        "$VALIDATION_DIR/Release/${RELEASE_NAME}.stderr.txt" \
        "$VALIDATION_DIR/Export/${RELEASE_NAME}.stderr.txt" \
        "$RELEASE_NAME build/export stderr"
done

if [ "$(uname -s)" = "Darwin" ]; then
    if printf 'q\n' | "$LAUNCHER" \
        >"$VALIDATION_DIR/Launcher/quit.txt" 2>&1; then
        :
    else
        foundation_die "macOS launcher failed to quit cleanly"
    fi
    grep -F 'Foundation Examples' "$VALIDATION_DIR/Launcher/quit.txt" >/dev/null || \
        foundation_die "macOS launcher did not print its heading"

    if printf '1\nn\n' | "$LAUNCHER" \
        >"$VALIDATION_DIR/Launcher/run-first.txt" 2>&1; then
        :
    else
        foundation_die "macOS launcher failed to run its first example"
    fi
    grep -F 'Process finished with exit code 0' \
        "$VALIDATION_DIR/Launcher/run-first.txt" >/dev/null || \
        foundation_die "macOS launcher did not report a successful example"
fi

printf '%s\n' \
    "Validated $EXPECTED_COUNT Debug, Release, and exported examples for $PRESET"
