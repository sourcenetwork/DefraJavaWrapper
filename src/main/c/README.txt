$ANDROID_NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang \
    -fPIC \
    -I${JAVA_HOME}/include \
    -I${JAVA_HOME}/include/linux \
    -L../jniLibs/arm64-v8a -ldefradb \
    -shared \
    -o ../jniLibs/arm64-v8a/libnativewrapper.so \
    nativewrapper.c
