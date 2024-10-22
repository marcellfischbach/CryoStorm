package org.cryo.core;

public class NoCsClassFoundException extends RuntimeException {
    public NoCsClassFoundException() {
    }

    public NoCsClassFoundException(String message) {
        super(message);
    }
}
