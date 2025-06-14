package org.crimsonedge.core.resource;

import org.crimsonedge.core.CeClass;
import org.crimsonedge.core.CoreObject;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.nio.ByteBuffer;

@CeClass("cs::csJavaFile")
public class csJavaFile extends CoreObject implements IFile {

    private final String name;

    private final byte[] data;

    private final InputStream inputStream;

    private int ptr;

    public csJavaFile(String name, InputStream inputStream) {
        super();
        this.name = name;
        this.inputStream = inputStream;
        this.data = readAll(inputStream);
        this.ptr = 0;
    }

    @Override
    public String getName() {
        return this.name;
    }

    public boolean open(int accessMode, int openMode) {
        return open(EAccessMode.values()[accessMode],
                    EOpenMode.values()[openMode]);
    }

    @Override
    public boolean open(EAccessMode accessMode, EOpenMode openMode) {
        return this.data != null && this.inputStream != null;
    }

    @Override
    public boolean isOpen() {
        return this.data != null && this.inputStream != null;
    }

    @Override
    public boolean isEOF() {
        return this.ptr >= this.data.length;
    }

    public void seek(int seekMode, long position) {
        seek(ESeekMode.values()[seekMode], position);
    }

    @Override
    public void seek(ESeekMode seekMode, long position) {
        switch (seekMode) {
            case SET:
                this.ptr = (int)position;
                break;
            case CUR:
                this.ptr += position;
                break;
            case END:
                this.ptr = this.data.length - (int)position;
                break;
        }
    }

    @Override
    public long tell() {
        return this.ptr;
    }

    @Override
    public int read(int elementSize, int elementCount, byte[] byteBuffer) {
        int numBytes = elementCount * elementSize;
        int remaining = this.data.length - this.ptr;
        int bytesRead = Math.min(numBytes, remaining);


        for (int i = 0; i < bytesRead; i++) {
            byteBuffer[i] = this.data[this.ptr++];
        }

        return bytesRead;
    }

    @Override
    public int write(int elementSize, int elementCount, ByteBuffer byteBuffer) {
        return 0;
    }

    @Override
    public void close() {
        try {
            this.inputStream.close();
        }
        catch (Exception e) {
            //
        }
    }


    private static byte[] readAll(InputStream inputStream) {
        try {
            // Create a ByteArrayOutputStream
            ByteArrayOutputStream buffer = new ByteArrayOutputStream();

            // Create a byte array for the buffer
            int nRead;
            byte[] data = new byte[1024];

            // Read the bytes from the input stream
            while ((nRead = inputStream.read(data, 0, data.length)) != -1) {
                buffer.write(data, 0, nRead);
            }

            // Get the byte array
            return buffer.toByteArray();
        }
        catch (Exception e) {
            return null;
        }
    }
}
