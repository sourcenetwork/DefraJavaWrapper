package source.defra;

public class DefraBLE {

    static {
        NativeLoader.load("nativewrapper");
        NativeLoader.load("defradb");
    }

    // Called by Java BLE driver when a peer is discovered via BLE.
    // Returns true if libp2p accepted the peer.
    public static native boolean handleFoundPeer(String remotePID);

    // Called by Java BLE driver when a peer is no longer reachable.
    public static native void handleLostPeer(String remotePID);

    // Called by Java BLE driver when data arrives from a peer over BLE.
    public static native void receiveFromPeer(String remotePID, byte[] payload);
	
	// Called by the user after creating a BLE interface in their Android project.
	public static native void registerBleInterface(Object bleInterface);

}