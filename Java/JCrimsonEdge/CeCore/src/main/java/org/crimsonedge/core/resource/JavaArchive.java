package org.crimsonedge.core.resource;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;

import java.io.File;
import java.io.InputStream;

@CeClass("ce::JavaArchive")
public class JavaArchive extends CoreObject implements IArchive {

    private int priority;

    private String rootPath;

    public JavaArchive() {
    }

    public JavaArchive(long ref) {
        super(ref);
    }

    public void setPriority(int priority) {
        this.priority = priority;
    }

    @Override
    public int getPriority() {
        return 1;
    }

    public String getRootPath() {
        return rootPath;
    }

    public void setRootPath(String rootPath) {
        this.rootPath = rootPath;
    }

    public IFile open(String locator, int accessMode, int openMode) {
        return open(locator, EAccessMode.values()[accessMode], EOpenMode.values()[openMode]);
    }

    @Override
    public IFile open(String locator, EAccessMode accessMode, EOpenMode openMode) {
        if (accessMode != EAccessMode.READ) {
            return null;
        }


        String path = this.rootPath + "/" + locator;
        InputStream resourceAsStream = JavaArchive.class.getResourceAsStream(path);
        if (resourceAsStream == null) {
            return null;
        }

        JavaFile file = new JavaFile(path, resourceAsStream);
        if (!file.open(accessMode, openMode)) {
            return null;
        }
        return file;
    }
}
