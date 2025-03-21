package org.cryo.lwjgl.window.input;

import org.cryo.core.CsClass;
import org.cryo.core.input.IInputSystem;
import org.cryo.core.input.IKeyboard;
import org.cryo.core.input.IMouse;

import static org.cryo.core.input.InputSystemJavaNative.*;


@CsClass("cs::csJavaInputSystem")
public class AwtGlCanvasInputSystem extends IInputSystem
{

	private final AwtGlCanvasKeyboard keyboard  = new AwtGlCanvasKeyboard();

	private final AwtGlCanvasMouse mouse = new AwtGlCanvasMouse();


	public AwtGlCanvasInputSystem()
	{
		nSetKeyboard(getRef(), refOf(keyboard));
		nSetMouse(getRef(), refOf(mouse));
	}

	@Override
	public AwtGlCanvasKeyboard getKeyboard()
	{
		return keyboard;
	}

	@Override
	public AwtGlCanvasMouse getMouse()
	{
		return mouse;
	}

}
