package org.cryo.core.resource;

public interface IArchive {

    int getPriority ();

    IFile open (String locator, EAccessMode accessMode, EOpenMode openMode);
}
