package msgbox;

import nlink.win32.DllClass;
import nlink.win32.DllMethod;

/**
 * @author Kohsuke Kawaguchi
 */
@DllClass
public interface User32 {
    @DllMethod
    int MessageBox(int hwnd, String text, String caption, int flags );
}
