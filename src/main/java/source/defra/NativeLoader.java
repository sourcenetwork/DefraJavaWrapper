package source.defra;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

class NativeLoader {

    private static final boolean IS_ANDROID =
            System.getProperty("java.vm.name", "").contains("Dalvik");

    // All native libraries that must be present in the temp dir.
    // libdefradb.so is loaded automatically as a DT_NEEDED dependency of
    // libnativewrapper.so via the $ORIGIN rpath embedded at build time,
    // so it must be extracted before nativewrapper is loaded.
    private static final String[] ALL_LIBS = {"defradb", "nativewrapper"};

    private static File tempDir;

    static synchronized void load(String libName) {
        if (IS_ANDROID) {
            System.loadLibrary(libName);
            return;
        }
        try {
            extractAll();
            // Only explicitly load nativewrapper — defradb is pulled in
            // automatically via the $ORIGIN rpath in libnativewrapper.so.
            if (libName.equals("nativewrapper")) {
                System.load(new File(tempDir, "libnativewrapper.so").getAbsolutePath());
            }
        } catch (IOException e) {
            throw new UnsatisfiedLinkError("Failed to extract native libraries: " + e.getMessage());
        }
    }

    private static void extractAll() throws IOException {
        if (tempDir != null) return;
        tempDir = new File(System.getProperty("java.io.tmpdir"), "defradb-" + System.nanoTime());
        tempDir.mkdirs();
        for (String lib : ALL_LIBS) {
            extractLib(lib);
        }
    }

    private static void extractLib(String libName) throws IOException {
        String fileName = "lib" + libName + ".so";
        File outFile = new File(tempDir, fileName);
        try (InputStream in = NativeLoader.class.getResourceAsStream("/" + fileName);
             FileOutputStream out = new FileOutputStream(outFile)) {
            if (in == null) {
                throw new UnsatisfiedLinkError("Native library not found in JAR: " + fileName);
            }
            byte[] buf = new byte[4096];
            int n;
            while ((n = in.read(buf)) != -1) out.write(buf, 0, n);
        }
    }
}
