package source.defra;

class DefraNewNodeResult {

	public long nodePtr;
    public String error;
    public int status;

    public DefraNewNodeResult(int status, String error, long ptr) {
        this.status = status;
        this.error = error;
        this.nodePtr = ptr;
    }
    
}
