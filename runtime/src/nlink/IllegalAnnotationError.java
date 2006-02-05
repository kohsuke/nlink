package nlink;

/**
 * Signals an error in the use of NLink annotations.
 *
 * @author Kohsuke Kawaguchi
 */
public class IllegalAnnotationError extends Error {
    public IllegalAnnotationError(String message) {
        super(message);
    }

    public IllegalAnnotationError(String message, Throwable cause) {
        super(message, cause);
    }

    public IllegalAnnotationError(Throwable cause) {
        super(cause);
    }
}
