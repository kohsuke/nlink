package nlink.win32;

import java.lang.annotation.Target;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import static java.lang.annotation.RetentionPolicy.RUNTIME;

/**
 * Checks {@code GetLastError} after the invocation and throws
 * {@link Win32NLinkException} if it returns non-zero.
 * 
 * @author Kohsuke Kawaguchi
 */
@Target({ElementType.METHOD})
@Retention(RUNTIME)
public @interface CheckLastError {
}
