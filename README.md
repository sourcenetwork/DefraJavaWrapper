# DefraJavaWrapper
**Building the Shared Objects for Android**

The pre-requisite for this working, is that the user has the Android NDK installed on his or her system, and that the environment variable `ANDROID_NDK` is set, appropriately, to its location. The build can then be invoked from the defradb directory with `make build-c-shared-android`. This will invoke a bash script located at `defradb/tools/scripts/build-c-shared-android.sh`.

This script does a few things. First, it adjusts all of the go files in the cbindings folder, changing “package cbindings” into “package main.” It sets up a trap so that upon finishing, whether successful or not, the package names are restored. The reason for this, is that building with CGO requires that the package be a main package – but our existing test code needs to be able to import the functions for use in our Go codebase. This workaround was proposed by Fred, and shown to work.

After the files are ready to build, two versions of the build command will run. One will create a .so at `build/arm64-v8a/libdefradb.so`, and the other will create an .so at `build/x86_64/libdefradb.so`. Both will be paired with a header file, although the header will be the same in both cases. So the script then copies the header to the base build directory, and removes the others. Additionally, it copies the necessary `defra_structs.h` file from the cbindings directory. After running the script, the user will be left with two header files, and two sub directories, each of which contains the shared object for a given architecture.

**Building the Shared Object for Linux**

Similar to building them for Android, a shared object can be built for Linux by running `make build-c-shared-linux`. This will invoke a bash script located at `defradb/tools/scripts/build-c-shared-Linux.sh`.


**Building the .aar For Android or the .jar for Linux**

This repo contains a the working directory of a gradle project that can be used to generate an .aar android archive, or a .jar for use on Linux. The higher level overview of what it's doing, is that it contains some C code that exposes functions to Java through JNI. It also contains Java code which will provide the functions and objects that users will use to interact with Defra. The assumption is that a recent version of Gradle is installed, and (again) that, in the case of the Android build, that the Android NDK is installed with the `ANDROID_NDK` environment variable set. Assuming the requirements are met, this is a several-step process, but not a complicated one:

1. Move `libdefradb.h` and `defra_structs.h` to the `src/main/c/` directory.
2a. For Android: Move the pair of shared objects generated in the "Building the Shared Objects for Android" step to `src/main/jniLibs/arm64-v8a' and `src/main/jniLibs/x86_64` respectively.
2b. For Linux: Move the shared objects generated in the "Building Shared Object for Linux" step to `src/main/linuxLibs`.
3. Inside the `src/main/c` directory, execute the existing `build.sh` script.
4a. Inside the root of the project, run `./gradlew -b build-linux.gradle build` or `./gradlew -b build-android.gradle build`
  
In step four, if you built for Android, the release and debug .aar archives will be created at `build/outputs/aar/defradb-android-release.aar`. This is the archive that can be used in an Android Studio project. If you built for Linux, the .jar will be created at `build/libs`.

**Using the .aar in Android Studio**

The following steps describe an example of how to utilize the .aar in an Android Studio project.

1. Place the `defradb-android-release.aar` file in the `<project>/app/src/libs/` directory.
2. Modify the `dependencyResolutionManagement` block of `settings.gradle` such that `repositoryies -> flatDir -> dirs` is set to the location of our .aar. For example:

```
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
        flatDir {
            dirs 'app/src/main/libs'
        }
    }
}
```

3. Modify the `android` block of `build.gradle (Module: app)` such that `android -> defaultConfig -> ndk -> abiFilters` match our supported architectures (arm64-v8a, and x86_64). For example:

```
android {
    namespace 'source.defra.wrapper2java'
    compileSdk 35

    defaultConfig {
        applicationId "source.defra.wrapper2java"
        minSdk 24
        targetSdk 35
        versionCode 1
        versionName "1.0"

        testInstrumentationRunner "androidx.test.runner.AndroidJUnitRunner"

        ndk {
            abiFilters "arm64-v8a", "x86_64"
        }
    }
```

4. In a .java file, you can import parts of the SDK as follows:

```
import source.defra.DefraCollectionOptions;
import source.defra.DefraIdentity;
import source.defra.DefraNode;
import source.defra.DefraNodeInitOptions;
import source.defra.DefraResult;
```

You will then be able to use them in your code.

**Using the .jar on Linux**

Suppose you have your Java code in a Java file called DefraTest.java. You can import and use the code the same way you could on Android. However, you will compile it the following way:

```
javac -cp defradb.jar DefraTest.java
```

And then you can run it:

```
java -cp .:defradb.jar DefraTest
```
