package org.crimsonedge.core.resource;

public interface IArchive {

    int getPriority ();

    IFile open (String locator, EAccessMode accessMode, EOpenMode openMode);
}
