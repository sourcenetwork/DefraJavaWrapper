package source.defra;

class DefraTransactionResult {

	public long txnPtr;
    public String error;
    public int status;

    public DefraTransactionResult(int status, String error, long ptr) {
        this.status = status;
        this.error = error;
        this.txnPtr = ptr;
    }
    
}
