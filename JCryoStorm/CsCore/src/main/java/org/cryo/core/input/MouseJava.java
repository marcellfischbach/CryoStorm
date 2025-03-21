package org.cryo.core.input;

import org.cryo.core.CsClass;

@CsClass("cs::csJavaMouse")
public class MouseJava extends IMouse
{

	public MouseJava()
	{
	}

	public MouseJava(long ref)
	{
		super(ref);
	}

	@Override
	public int getX()
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

	@Override
	public int getY()
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

	@Override
	public int getDeltaX()
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

	@Override
	public int getDeltaY()
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

	@Override
	public int getWheel()
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

	@Override
	public int getWheelHorizontal()
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

	public boolean isButtonDown(int key)
	{
		return isButtonDown(EMouseButton.values()[key]);
	}

	public boolean isButtonUp(int key)
	{
		return isButtonUp(EMouseButton.values()[key]);
	}

	public boolean isButtonPressed(int key)
	{
		return isButtonDown(EMouseButton.values()[key]);
	}

	public boolean isButtonReleased(int key)
	{
		return isButtonDown(EMouseButton.values()[key]);
	}

	@Override
	public boolean isButtonDown(EMouseButton button)
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

	@Override
	public boolean isButtonUp(EMouseButton button)
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

	@Override
	public boolean isButtonPressed(EMouseButton button)
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

	@Override
	public boolean isButtonReleased(EMouseButton button)
	{
		throw new UnsupportedOperationException("To be implemented in subclass");
	}

}
