package org.cryo.core.input;

import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

import static org.cryo.core.input.IKeyboardNative.*;

@CsClass(CS_CLASS_NAME)
public class IKeyboard extends CsObject
{

	protected IKeyboard()
	{
	}

	public IKeyboard(long ref)
	{
		super(ref);
	}

	public boolean isKeyDown(EKey key)
	{
		return nIsKeyDown(getRef(), key.ordinal());
	}

	public boolean isKeyUp(EKey key)
	{
		return nIsKeyUp(getRef(), key.ordinal());
	}

	public boolean isKeyPressed(EKey key)
	{
		return nIsKeyPressed(getRef(), key.ordinal());
	}

	public boolean isKeyReleased(EKey key)
	{
		return nIsKeyReleased(getRef(), key.ordinal());
	}


}
