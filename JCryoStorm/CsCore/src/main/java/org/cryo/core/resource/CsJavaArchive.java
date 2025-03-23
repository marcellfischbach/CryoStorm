package org.cryo.core.resource;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

import java.io.InputStream;


@CsClass("cs::csJavaArchive")
public class CsJavaArchive extends CsObject implements IArchive {

    private int priority;

    private String rootPath;

    public CsJavaArchive() {
    }

    public CsJavaArchive(long ref) {
        super(ref);
    }

    public void setPriority(int priority) {
        this.priority = priority;
    }

    @Override
    public int getPriority() {
        return this.priority;
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


        String path = this.rootPath + locator;
        InputStream resourceAsStream = CsJavaArchive.class.getResourceAsStream(path);
        if (resourceAsStream == null) {
            return null;
        }

        CsJavaFile file = new CsJavaFile(path, resourceAsStream);
        if (!file.open(accessMode, openMode)) {
            System.err.println("JavaArchive.open failed 1 '" + path + "'");
            return null;
        }

        return file;
    }
}
