package org.cryo.core.input;

import org.cryo.core.CsObject;

import static org.cryo.core.input.IMouseNative.*;

public class IMouse extends CsObject
{

	protected IMouse()
	{
	}

	public IMouse(long ref)
	{
		super(ref);
	}

	public int getX()
	{
		return nGetX(getRef());
	}

	public int getY()
	{
		return nGetY(getRef());
	}

	public int getDeltaX()
	{
		return nGetDeltaX(getRef());
	}

	public int getDeltaY()
	{
		return nGetDeltaY(getRef());
	}

	public int getWheel()
	{
		return nGetWheel(getRef());
	}

	public int getWheelHorizontal()
	{
		return nGetWheelHorizontal(getRef());
	}

	public boolean isButtonDown(EMouseButton button)
	{
		return nIsButtonDown(getRef(), button.ordinal());
	}

	public boolean isButtonUp(EMouseButton button)
	{
		return nIsButtonUp(getRef(), button.ordinal());
	}

	public boolean isButtonPressed(EMouseButton button)
	{
		return nIsButtonPressed(getRef(), button.ordinal());
	}

	public boolean isButtonReleased(EMouseButton button)
	{
		return nIsButtonReleased(getRef(), button.ordinal());
	}


}
