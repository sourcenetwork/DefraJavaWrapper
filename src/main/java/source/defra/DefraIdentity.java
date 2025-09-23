package source.defra;

public class DefraIdentity {

    private long ptr;

    public DefraIdentity(long ptr) {
        this.ptr = ptr;
    }

    public long getPointer() {
        return this.ptr;
    }
    
}
