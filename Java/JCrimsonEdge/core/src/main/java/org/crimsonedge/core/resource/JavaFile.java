package org.crimsonedge.core.resource;

import org.crimsonedge.core.CoreObject;

import java.io.InputStream;
import java.nio.ByteBuffer;

public class JavaFile extends CoreObject implements IFile {

    public JavaFile(InputStream inputStream) {
    }

    @Override
    public String getName() {
        return "";
    }

    @Override
    public boolean open(EAccessMode accessMode, EOpenMode openMode) {
        return false;
    }

    @Override
    public boolean isOpen() {
        return false;
    }

    @Override
    public boolean isEOF() {
        return false;
    }

    @Override
    public void seek(ESeekMode seekMode, long offset) {

    }

    @Override
    public long tell() {
        return 0;
    }

    @Override
    public int read(int elementSize, int elementCount, ByteBuffer byteBuffer) {
        return 0;
    }

    @Override
    public int write(int elementSize, int elementCount, ByteBuffer byteBuffer) {
        return 0;
    }

    @Override
    public void close() {

    }
}
