#!/bin/bash
set -euo pipefail

android_present=false
linux_present=false

while [[ $# -gt 0 ]]; do
    case "$1" in
        --android) android_present=true; shift ;;
        --linux) linux_present=true; shift ;;
        *) echo "Unknown argument: $1"; exit 1 ;;
    esac
done

NDK=$ANDROID_NDK
JNI=../jniLibs

if [ "$android_present" = true ]; then
    mkdir -p $JNI/arm64-v8a
    $NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang \
        -fPIC \
        -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux \
        -L$JNI/arm64-v8a -ldefradb \
        -shared -o $JNI/arm64-v8a/libnativewrapper.so \
        nativewrapper.c

    mkdir -p $JNI/x86_64
    $NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/x86_64-linux-android21-clang \
        -fPIC \
        -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux \
        -L$JNI/x86_64 -ldefradb \
        -shared -o $JNI/x86_64/libnativewrapper.so \
        nativewrapper.c
fi

if [ "$linux_present" = true ]; then
    LINUX_OUT=../linuxLibs
    mkdir -p $LINUX_OUT
    gcc \
        -fPIC \
        -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux \
        -Wl,-rpath,'$ORIGIN' \
        -shared \
        -o $LINUX_OUT/libnativewrapper.so \
        nativewrapper.c \
        -L$LINUX_OUT \
        -ldefradb
fi