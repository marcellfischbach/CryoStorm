package org.cryo.core.input;

import org.cryo.core.math.CsVector2f;

public class CsInput
{

	private static native void nInvalidate ();

	private static native Object nGetKeyboard ();
	private static native Object nGetMouse ();

	public static void invalidate ()
	{
		nInvalidate();
	}

	public static IMouse getMouse ()
	{
		return (IMouse)nGetMouse();
	}


	public static IKeyboard getKeyboard ()
	{
		return (IKeyboard)nGetKeyboard();
	}


	private static CsVector2f mousePosition = new CsVector2f();
	public static CsVector2f getMousePosition ()
	{
		return getMousePosition(mousePosition);
	}

	public static CsVector2f getMousePosition (CsVector2f result)
	{
		IMouse mouse = getMouse();
		result.set(mouse.getX(), mouse.getY());
		return result;
	}


	private static CsVector2f mouseDelta = new CsVector2f();
	public static CsVector2f getMouseDelta ()
	{
		return getMouseDelta(mouseDelta);
	}

	public static CsVector2f getMouseDelta (CsVector2f result)
	{
		IMouse mouse = getMouse();
		result.set(mouse.getDeltaX(), mouse.getDeltaY());
		return result;
	}
}
