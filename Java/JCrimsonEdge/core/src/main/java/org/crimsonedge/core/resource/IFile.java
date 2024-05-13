package org.crimsonedge.core.resource;

import java.nio.ByteBuffer;

public interface IFile {

    String getName ();

    boolean open(EAccessMode accessMode, EOpenMode openMode);

    boolean isOpen ();

    boolean isEOF ();

    void seek (ESeekMode seekMode, long offset);

    long tell();

    int read (int elementSize, int elementCount, byte[] byteBuffer);

    int write (int elementSize, int elementCount, ByteBuffer byteBuffer);

    void close();

}
