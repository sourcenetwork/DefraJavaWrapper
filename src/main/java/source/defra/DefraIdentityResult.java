package source.defra;

public class DefraIdentityResult {

	public long identityPtr;
    public String error;
    public int status;

    public DefraIdentityResult(int status, String error, long ptr) {
        this.status = status;
        this.error = error;
        this.identityPtr = ptr;
    }
    
}
