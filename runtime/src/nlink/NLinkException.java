package nlink;

/**
 * Signals a failure in the COM method invocation.
 *
 * <p>
 * Calling a wrapped COM method throws this exception
 * when the underlying COM method returns a failure HRESULT code.
 *
 * @author Kohsuke Kawaguchi (kk@kohsuke.org)
 */
public class NLinkException extends RuntimeException {
    private final String fileName;
    private final int line;

    public NLinkException( String msg, String fileName, int line ) {
        super(msg);
        this.fileName = fileName;
        this.line = line;
    }

    public NLinkException(String message) {
        this(message,null,-1);
    }

    public NLinkException(String message, Throwable cause) {
        this(message);
        initCause(cause);
    }

    public NLinkException(Throwable cause) {
        this(cause.getMessage());
        initCause(cause);
    }
    
    @Override
    public String toString() {
        String s = super.toString();
        if(fileName!=null) {
            s += " : "+fileName+':'+line;
        }
        return s;
    }
}
