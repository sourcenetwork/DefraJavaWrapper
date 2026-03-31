package source.defra;

public class DefraException extends RuntimeException {
    public DefraException(String message) {
        super(message);
    }

    public DefraException(String message, Throwable cause) {
        super(message, cause);
    }
}