package source.defra;

class DefraResult {

	public String value;
    public String error;
    public int status;

    public DefraResult(int status, String error, String value) {
        this.status = status;
        this.error = error;
        this.value = value;
    }
    
}
