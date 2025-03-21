package org.cryo.core.input;

import org.cryo.core.CsClass;

@CsClass("cs::csJavaKeyboard")
public class KeyboardJava extends IKeyboard
{

	public KeyboardJava()
	{
	}

	public KeyboardJava(long ref)
	{
		super(ref);
	}

	public boolean isKeyDown(int key)
	{
		return isKeyDown(EKey.values()[key]);
	}

	public boolean isKeyUp(int key)
	{
		return isKeyUp(EKey.values()[key]);
	}

	public boolean isKeyPressed(int key)
	{
		return isKeyDown(EKey.values()[key]);
	}

	public boolean isKeyReleased(int key)
	{
		return isKeyDown(EKey.values()[key]);
	}


	@Override
	public boolean isKeyDown(EKey key)
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

	@Override
	public boolean isKeyUp(EKey key)
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

	@Override
	public boolean isKeyPressed(EKey key)
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

	@Override
	public boolean isKeyReleased(EKey key)
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

}
