package nlink.win32;

import java.lang.annotation.Target;
import java.lang.annotation.ElementType;

/**
 * Checks {@code GetLastError} after the invocation and throws
 * {@link Win32NLinkException} if it returns non-zero.
 * 
 * @author Kohsuke Kawaguchi
 */
@Target({ElementType.METHOD})
public @interface CheckLastError {
}
