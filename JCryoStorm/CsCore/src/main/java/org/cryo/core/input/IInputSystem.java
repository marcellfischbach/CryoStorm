package org.cryo.core.input;


import org.cryo.core.CsClass;
import org.cryo.core.CsObject;

import static org.cryo.core.input.IInputSystemNative.*;

@CsClass(IInputSystemNative.CS_CLASS_NAME)
public class IInputSystem extends CsObject
{

	public IInputSystem()
	{
	}

	public IInputSystem(long ref)
	{
		super(ref);
	}

	public IKeyboard getKeyboard ()
	{
		return (IKeyboard) nGetKeyboard(getRef());
	}

	public IMouse getMouse ()
	{
		return (IMouse) nGetMouse(getRef());
	}


}
