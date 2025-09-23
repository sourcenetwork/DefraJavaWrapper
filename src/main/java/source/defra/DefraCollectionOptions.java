package source.defra;

public class DefraCollectionOptions {

    public String version;
    public String collectionID;
    public String name;
    public DefraIdentity identity;
    public boolean getInactive;

    public DefraCollectionOptions() {
        this.version = "";
        this.collectionID = "";
        this.name = "";
        this.identity = null;
        this.getInactive = false;
    }
    
}
