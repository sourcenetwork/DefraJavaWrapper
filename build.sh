#!/bin/bash
android_present=false
linux_present=false
ble_enabled=false
cleanup=false
defra_dir=""
script_dir="$(pwd)"

# -------------------------
# ARG PARSING
# -------------------------
while [[ $# -gt 0 ]]; do
    case "$1" in
        --android)
            android_present=true
            shift
            ;;
        --linux)
            linux_present=true
            shift
            ;;
        --cleanup)
            cleanup=true
            shift
            ;;
        --defra-dir)
            if [ -z "$2" ]; then
                echo "Error: --defra-dir requires a path"
                exit 1
            fi
            defra_dir="$2"
            shift 2
            ;;
        *)
            echo "Unknown argument: $1"
            exit 1
            ;;
    esac
done

if [ -z "$defra_dir" ]; then
    echo "Error: --defra-dir is required"
    echo "Usage: ./build.sh [ --android ] [ --linux ] [ --cleanup ] --defra-dir <path>"
    exit 1
fi

defra_dir_abs="$defra_dir"

mkdir -p "$script_dir/src/main/linuxLibs"
mkdir -p "$script_dir/src/main/jniLibs/arm64-v8a"
mkdir -p "$script_dir/src/main/jniLibs/x86_64"
mkdir -p "$script_dir/src/main/c"

# -------------------------
# ANDROID BUILD
# -------------------------
if [ "$android_present" = true ]; then
    echo "Running Android build..."
    (cd "$defra_dir_abs" && make build-c-shared-android)
    if [ $? -ne 0 ]; then
        echo "make build-c-shared-android FAILED"
        exit 1
    fi

    if [ -f "$defra_dir_abs/build/arm64-v8a/libdefradb.so" ]; then
        echo "Android arm64-v8a build successful"
        cp "$defra_dir_abs/build/arm64-v8a/libdefradb.so" "$script_dir/src/main/jniLibs/arm64-v8a/"
        echo "Copied ARM64 .so"
    else
        echo "Android arm64-v8a build FAILED"
        exit 1
    fi

    if [ -f "$defra_dir_abs/build/x86_64/libdefradb.so" ]; then
        echo "Android x86_64 build successful"
        cp "$defra_dir_abs/build/x86_64/libdefradb.so" "$script_dir/src/main/jniLibs/x86_64/"
        echo "Copied x86_64 .so"
    else
        echo "Android x86_64 build FAILED"
        exit 1
    fi

    cp "$defra_dir_abs/build/libdefradb.h" "$script_dir/src/main/c/"
    cp "$defra_dir_abs/build/defra_structs.h" "$script_dir/src/main/c/"
    echo "Copied Android headers"

    echo "Running C build for Android..."
    if [ -f "$script_dir/src/main/c/build.sh" ]; then
        chmod +x "$script_dir/src/main/c/build.sh"
        BLE_FLAG=""
        [ "$ble_enabled" = true ] && BLE_FLAG="--ble-enabled"
        (cd "$script_dir/src/main/c" && ./build.sh --android)
        echo "Android C build completed"
    else
        echo "Warning: src/main/c/build.sh not found"
    fi

    echo "Running Android Gradle build..."
    "$script_dir/gradlew" -b "$script_dir/build-android.gradle" build
    if [ $? -eq 0 ]; then
        echo "Android Gradle build successful"
    else
        echo "Android Gradle build FAILED"
        exit 1
    fi
fi

# -------------------------
# LINUX BUILD
# -------------------------
if [ "$linux_present" = true ]; then
    echo "Running Linux build..."
    (cd "$defra_dir_abs" && make build-c-shared-linux)
    if [ $? -ne 0 ]; then
        echo "make build-c-shared-linux FAILED"
        exit 1
    fi

    if [ -f "$defra_dir_abs/build/libdefradb.so" ]; then
        echo "Linux build successful"
        cp "$defra_dir_abs/build/libdefradb.so" "$script_dir/src/main/linuxLibs/"
        echo "Copied Linux .so"
    else
        echo "Linux build FAILED"
        exit 1
    fi

    cp "$defra_dir_abs/build/libdefradb.h" "$script_dir/src/main/c/"
    cp "$defra_dir_abs/build/defra_structs.h" "$script_dir/src/main/c/"
    echo "Copied Linux headers"

    echo "Running C build for Linux..."
    if [ -f "$script_dir/src/main/c/build.sh" ]; then
        chmod +x "$script_dir/src/main/c/build.sh"
        (cd "$script_dir/src/main/c" && ./build.sh --linux)
        echo "Linux C build completed"
    else
        echo "Warning: src/main/c/build.sh not found"
    fi

    echo "Running Linux Gradle build..."
    "$script_dir/gradlew" -b "$script_dir/build-linux.gradle" build
    if [ $? -eq 0 ]; then
        echo "Linux Gradle build successful"
    else
        echo "Linux Gradle build FAILED"
        exit 1
    fi
fi

# -------------------------
# CLEANUP
# -------------------------
if [ "$cleanup" = true ]; then
    echo "Cleaning up build artifacts..."
    rm -f "$script_dir/src/main/c/libdefradb.h"
    rm -f "$script_dir/src/main/c/defra_structs.h"
    rm -f "$script_dir/src/main/linuxLibs/libdefradb.so"
	rm -f "$script_dir/src/main/linuxLibs/libnativewrapper.so"
    rm -f "$script_dir/src/main/jniLibs/arm64-v8a/libdefradb.so"
    rm -f "$script_dir/src/main/jniLibs/x86_64/libdefradb.so"
	rm -f "$script_dir/src/main/jniLibs/arm64-v8a/libnativvewrapper.so"
    rm -f "$script_dir/src/main/jniLibs/x86_64/libnativewrapper.so"
    echo "Cleanup complete"
fi