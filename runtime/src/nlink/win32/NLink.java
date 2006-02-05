package nlink.win32;

import nlink.NLinkException;
import nlink.IllegalAnnotationError;

import java.io.File;
import java.lang.reflect.Proxy;

/**
 * @author Kohsuke Kawaguchi
 */
public abstract class NLink {
    private NLink() {} // no instantiation please

    public static <T> T create(Class<T> dllInterface) {
        return create(dllInterface,null);
    }

    public static <T> T create(Class<T> dllInterface, File library) {
        if(!dllInterface.isInterface())
            throw new NLinkException(dllInterface+" is not an interface");

        DllClass dc = dllInterface.getAnnotation(DllClass.class);
        if(dc==null)
            throw new NLinkException(dllInterface+" does not have @DllClass annotation");

        String libPath;
        if (library != null) {
            libPath = library.toString();
        } else {
            libPath = dc.value();
            if(libPath.equals("")) {
                // default to the class name
                libPath = dllInterface.getSimpleName();
            }
        }

        return dllInterface.cast(Proxy.newProxyInstance( dllInterface.getClassLoader(),
            new Class[]{dllInterface}, new Wrapper(dllInterface,libPath) ));
    }
}
