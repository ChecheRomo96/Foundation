#!/bin/sh
set -eu

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
. "$SCRIPT_DIR/common.sh"

usage() {
    printf '%s\n' "Usage: $0 <preset> [--package <prefix>]"
}

PRESET=""
PACKAGE_PREFIX=""

while [ "$#" -gt 0 ]; do
    case "$1" in
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
foundation_require_configured "$PRESET"

[ -n "$PACKAGE_PREFIX" ] || PACKAGE_PREFIX="$FOUNDATION_DIST_ROOT/$PRESET"
PACKAGE_PREFIX=$(foundation_absolute_path "$PACKAGE_PREFIX")
[ -d "$PACKAGE_PREFIX" ] || \
    foundation_die "package prefix not found: $PACKAGE_PREFIX"

BUILD_DIR=$(foundation_build_dir "$PRESET")
BUILD_INFO="$PACKAGE_PREFIX/lib/cmake/Foundation/FoundationBuildInfo.cmake"
FOUNDATION_CACHE="$BUILD_DIR/CMakeCache.txt"

cmake \
    "-DPRESET=$PRESET" \
    "-DBUILD_DIR=$BUILD_DIR" \
    "-DPACKAGE_PREFIX=$PACKAGE_PREFIX" \
    -P "$FOUNDATION_ROOT/tests/cmake/PackageIdentity.cmake"

foundation_package_info_value() {
    sed -n "s/^set($1 \"\\(.*\\)\")$/\\1/p" "$BUILD_INFO" | sed -n '1p'
}

foundation_cache_value() {
    sed -n "s/^$1:[^=]*=//p" "$FOUNDATION_CACHE" | sed -n '1p'
}

SYSTEM_NAME=$(foundation_package_info_value Foundation_SYSTEM_NAME)
SYSTEM_PROCESSOR=$(foundation_package_info_value Foundation_SYSTEM_PROCESSOR)
OSX_ARCHITECTURES=$(foundation_package_info_value Foundation_OSX_ARCHITECTURES)
ARM_CPU=$(foundation_package_info_value Foundation_ARM_CPU)
ARM_FLOAT_ABI=$(foundation_package_info_value Foundation_ARM_FLOAT_ABI)
AVR_ARCHITECTURE=$(foundation_package_info_value Foundation_AVR_ARCHITECTURE)
OBJDUMP=$(foundation_cache_value CMAKE_OBJDUMP)
READELF=$(foundation_cache_value CMAKE_READELF)
ARCHIVE="$PACKAGE_PREFIX/lib/libFoundation.a"

case "$SYSTEM_NAME" in
    Darwin)
        command -v lipo >/dev/null 2>&1 || \
            foundation_die "lipo is required to validate a macOS archive"
        ACTUAL_ARCHITECTURES=$(lipo -archs "$ARCHIVE")
        EXPECTED_ARCHITECTURES=$(printf '%s' "$OSX_ARCHITECTURES" | tr ';' ' ')
        [ "$ACTUAL_ARCHITECTURES" = "$EXPECTED_ARCHITECTURES" ] || \
            foundation_die "archive architecture '$ACTUAL_ARCHITECTURES' does not match '$EXPECTED_ARCHITECTURES'"
        printf '%s\n' "Validated Mach-O archive architecture: $ACTUAL_ARCHITECTURES"
        ;;
    Linux)
        [ -x "$OBJDUMP" ] || foundation_die "CMAKE_OBJDUMP is unavailable: $OBJDUMP"
        INSPECTION=$($OBJDUMP -f "$ARCHIVE" 2>&1)
        printf '%s\n' "$INSPECTION"
        case "$SYSTEM_PROCESSOR" in
            x86_64|AMD64|amd64)
                printf '%s\n' "$INSPECTION" | \
                    grep -E 'architecture: i386:x86-64|file format elf64-x86-64' >/dev/null || \
                    foundation_die "archive is not Linux x86-64"
                ;;
            aarch64|arm64)
                printf '%s\n' "$INSPECTION" | \
                    grep -E 'architecture: aarch64|file format elf64-littleaarch64' >/dev/null || \
                    foundation_die "archive is not Linux Arm64"
                ;;
            *)
                foundation_die "unsupported Linux processor identity: $SYSTEM_PROCESSOR"
                ;;
        esac
        printf '%s\n' "Validated ELF archive architecture: $SYSTEM_PROCESSOR"
        ;;
    Generic)
        [ -x "$OBJDUMP" ] || foundation_die "CMAKE_OBJDUMP is unavailable: $OBJDUMP"
        INSPECTION=$($OBJDUMP -f "$ARCHIVE" 2>&1)
        printf '%s\n' "$INSPECTION"

        if [ -n "$AVR_ARCHITECTURE" ]; then
            EXPECTED_AVR_ARCHITECTURE=$(printf '%s' "$AVR_ARCHITECTURE" | \
                sed 's/^avr/avr:/')
            printf '%s\n' "$INSPECTION" | grep -F 'file format elf32-avr' >/dev/null || \
                foundation_die "archive is not AVR ELF"
            printf '%s\n' "$INSPECTION" | \
                grep -F "architecture: $EXPECTED_AVR_ARCHITECTURE" >/dev/null || \
                foundation_die "archive is not $AVR_ARCHITECTURE"
            printf '%s\n' "Validated AVR archive architecture: $AVR_ARCHITECTURE"
        elif [ -n "$ARM_CPU" ]; then
            printf '%s\n' "$INSPECTION" | \
                grep -F 'file format elf32-littlearm' >/dev/null || \
                foundation_die "archive is not 32-bit little-endian Arm ELF"
            printf '%s\n' "$INSPECTION" | grep -F 'architecture: arm' >/dev/null || \
                foundation_die "archive does not contain Arm objects"

            [ -x "$READELF" ] || foundation_die "CMAKE_READELF is unavailable: $READELF"
            ATTRIBUTES=$($READELF -A "$ARCHIVE" 2>&1)
            printf '%s\n' "$ATTRIBUTES"
            if [ "$ARM_FLOAT_ABI" = "hard" ]; then
                printf '%s\n' "$ATTRIBUTES" | \
                    grep -F 'Tag_ABI_VFP_args: VFP registers' >/dev/null || \
                    foundation_die "hard-float archive lacks the VFP register ABI attribute"
            elif printf '%s\n' "$ATTRIBUTES" | \
                grep -F 'Tag_ABI_VFP_args: VFP registers' >/dev/null; then
                foundation_die "soft-float archive advertises the hard-float VFP register ABI"
            fi
            printf '%s\n' "Validated Arm archive identity: $ARM_CPU / $ARM_FLOAT_ABI"
        else
            foundation_die "Generic package declares neither an Arm nor AVR identity"
        fi
        ;;
    *)
        foundation_die "unsupported package system for this validator: $SYSTEM_NAME"
        ;;
esac

printf '%s\n' "Validated Foundation package identity at $PACKAGE_PREFIX"
