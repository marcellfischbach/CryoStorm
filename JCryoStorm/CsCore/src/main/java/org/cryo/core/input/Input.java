package org.cryo.core.input;

import org.cryo.core.math.Vector2f;
import org.cryo.core.math.Vector3f;

public class Input
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


	private static Vector2f mousePosition = new Vector2f();
	public static Vector2f getMousePosition ()
	{
		return getMousePosition(mousePosition);
	}

	public static Vector2f getMousePosition (Vector2f result)
	{
		IMouse mouse = getMouse();
		result.set(mouse.getX(), mouse.getY());
		return result;
	}


	private static Vector2f mouseDelta = new Vector2f();
	public static Vector2f getMouseDelta ()
	{
		return getMouseDelta(mouseDelta);
	}

	public static Vector2f getMouseDelta (Vector2f result)
	{
		IMouse mouse = getMouse();
		result.set(mouse.getDeltaX(), mouse.getDeltaY());
		return result;
	}
}
