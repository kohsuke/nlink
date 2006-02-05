package nlink.win32;

import java.net.URL;
import java.io.File;

/**
 * @author Kohsuke Kawaguchi
 */
class Native {
    static native int loadLibrary(String name);

    static native int getLastError();
    static native String formatErrorMessage(int errorCode);

    /**
     * Invokes a method.
     */
    static native Object invoke( int functionPtr,
                                 Object[] args, int[] parameterConversions,
                                 Class returnType, int returnConversion );

    static native int getProcAddress2(int hModule, int integer);
    static native int getProcAddress(int hModule, String name);

    static {
        loadNativeLibrary();
    }

    static void loadNativeLibrary() {
        try {
            // load the native part of the code.
            // first try java.library.path
            System.loadLibrary("nlink");
            return;
        } catch( Throwable t ) {
            ;
        }

        // try loading com4j.dll in the same directory as com4j.jar
        URL res = Native.class.getClassLoader().getResource("nlink/Const.class");
        String url = res.toExternalForm();
        if(url.startsWith("jar://")) {
            int idx = url.lastIndexOf('!');
            String filePortion = url.substring(6,idx);
            if(filePortion.startsWith("file://")) {
                File jarFile = new File(filePortion.substring(7));
                File dllFile = new File(jarFile.getParentFile(),"nlink.dll");
                System.load(dllFile.getPath());
                return;
            }
        }

        throw new UnsatisfiedLinkError("Unable to load nlink.dll");
    }
}
