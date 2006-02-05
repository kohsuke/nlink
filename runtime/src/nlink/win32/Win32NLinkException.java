package nlink.win32;

import nlink.NLinkException;

/**
 * Signals an error during the NLink processing.
 *
 * @author Kohsuke Kawaguchi
 */
public class Win32NLinkException extends NLinkException {
    /**
     * The value from {@code GetLastError}.
     */
    private final int errorCode = Native.getLastError();

    /**
     * The formatter message of {@link #errorCode}
     */
    private final String errorCodeMessage = Native.formatErrorMessage(errorCode);


    public Win32NLinkException(String msg, String fileName, int line) {
        super(msg, fileName, line);

    }

    public Win32NLinkException(String message) {
        super(message);
    }

    @Override
    public String getMessage() {
        if(errorCodeMessage==null) {
            return super.getMessage();
        }
        return super.getMessage()+" : "+errorCodeMessage;
    }
}
