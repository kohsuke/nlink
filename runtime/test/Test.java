import nlink.win32.NLink;

/**
 * @author Kohsuke Kawaguchi
 */
public class Test {
    public static void main(String[] args) {
        User32 user32 = NLink.create(User32.class);
        System.out.println(user32.MessageBox(0,"Hello","Testing",4|0x20));
    }
}
