package source.defra;

public class DefraNodeInitOptions {

    public String dbPath;
    public String listeningAddresses;
    public String replicatorRetryIntervals;
    public String peers;
    public DefraIdentity identity;
    public boolean inMemory;
    public boolean disableP2P;
    public boolean disableAPI;
    public boolean enableNodeACP;
    public int maxTransactionRetries;

    public DefraNodeInitOptions() {
        this.dbPath = "";
        this.listeningAddresses = "";
        this.replicatorRetryIntervals = "";
        this.peers = "";
        this.identity = null;
        this.inMemory = false;
        this.disableP2P = true;
        this.disableAPI = true;
        this.enableNodeACP = false;
        this.maxTransactionRetries = 0;
    }
    
}
