#!/bin/bash
set -euo pipefail

NDK=$ANDROID_NDK
JNI=../jniLibs

# ARM64-v8a
mkdir -p $JNI/arm64-v8a
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang \
    -fPIC \
    -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux \
    -L$JNI/arm64-v8a -ldefradb \
    -shared -o $JNI/arm64-v8a/libnativewrapper.so \
    nativewrapper.c

# x86_64
mkdir -p $JNI/x86_64
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/x86_64-linux-android21-clang \
    -fPIC \
    -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux \
    -L$JNI/x86_64 -ldefradb \
    -shared -o $JNI/x86_64/libnativewrapper.so \
    nativewrapper.c
