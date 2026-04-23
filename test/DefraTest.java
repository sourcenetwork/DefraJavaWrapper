import source.defra.DefraException;
import source.defra.DefraNode;
import source.defra.DefraNodeInitOptions;

public class DefraTest {
    public static void main(String[] args) {
        DefraNodeInitOptions options = new DefraNodeInitOptions();
        options.inMemory = true;
        try {
            System.out.println("Creating DefraNode...");
            DefraNode node = new DefraNode(options);
            System.out.println("DefraNode created successfully.");
            node.close();
            System.out.println("DefraNode closed successfully.");
            System.out.println("Test passed.");
        } catch (DefraException e) {
            System.err.println("Test failed: " + e.getMessage());
            System.exit(1);
        }
    }
}
