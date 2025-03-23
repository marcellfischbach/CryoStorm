package org.cryo.core.resource;


import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.nio.ByteBuffer;

import static org.cryo.core.resource.CsJavaFileNative.*;

@CsClass(CS_CLASS_NAME)
public class CsJavaFile extends CsObject implements IFile {

    private final String name;

    private final byte[] data;

    private final InputStream inputStream;

    private int ptr;

    public CsJavaFile(String name, InputStream inputStream) {
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

    public void seek(int seekMode, long offset) {
        seek(ESeekMode.values()[seekMode], offset);
    }

    @Override
    public void seek(ESeekMode seekMode, long offset) {
        switch (seekMode) {
            case SET:
                this.ptr = (int) offset;
                break;
            case CUR:
                this.ptr += offset;
                break;
            case END:
                this.ptr = this.data.length - (int) offset;
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
