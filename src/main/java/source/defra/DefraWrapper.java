package source.defra;

public class DefraWrapper {
    static {
        System.loadLibrary("defradb");
        System.loadLibrary("nativewrapper");
    }

    // JNI declarations

    // Callable functions

    public DefraNode NewNode(DefraNodeInitOptions options) {
        return new DefraNode(options);
    }
    
}
