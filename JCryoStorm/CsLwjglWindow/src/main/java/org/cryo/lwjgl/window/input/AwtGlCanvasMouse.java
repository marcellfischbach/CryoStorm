package org.cryo.lwjgl.window.input;

import org.cryo.core.CsClass;
import org.cryo.core.input.EKey;
import org.cryo.core.input.EMouseButton;
import org.cryo.core.input.MouseJava;
import org.cryo.core.input.MouseJavaNative;

@CsClass(MouseJavaNative.CS_CLASS_NAME)
public class AwtGlCanvasMouse extends MouseJava
{

	private int primedX;
	private int primedY;


	private int x;
	private int y;

	private int relX;
	private int relY;

	private int wheel;
	private int wheelHorizontal;

	private boolean[] primedButton = new boolean[EMouseButton.values().length];
	private boolean[] current = new boolean[EMouseButton.values().length];
	private boolean[] prev = new boolean[EMouseButton.values().length];


	public AwtGlCanvasMouse()
	{
	}

	public void update()
	{
		this.relX = this.primedX - this.x;
		this.relY = this.primedY - this.y;
		this.x = this.primedX;
		this.y = this.primedY;

		System.arraycopy(current, 0, prev, 0, current.length);
		System.arraycopy(primedButton, 0, current, 0, current.length);


		wheel = 0;
		wheelHorizontal = 0;
	}



	public void primeButton(int button, boolean down)
	{
		int cryoButton = button - 1;
		if (cryoButton >= 0 && cryoButton < EKey.values().length)
		{
			this.primedButton[cryoButton] = down;
		}
	}

	public void primePosition(int x, int y)
	{
		this.primedX = x;
		this.primedY = y;
	}


	@Override
	public int getX()
	{
		return x;
	}

	@Override
	public int getY()
	{
		return y;
	}

	@Override
	public int getDeltaX()
	{
		return relX;
	}

	@Override
	public int getDeltaY()
	{
		return relY;
	}

	@Override
	public int getWheel()
	{
		return wheel;
	}

	@Override
	public int getWheelHorizontal()
	{
		return wheelHorizontal;
	}

	@Override
	public boolean isButtonDown(EMouseButton button)
	{
		return current[button.ordinal()];
	}

	@Override
	public boolean isButtonUp(EMouseButton button)
	{
		return !current[button.ordinal()];
	}

	@Override
	public boolean isButtonPressed(EMouseButton button)
	{
		return current[button.ordinal()] && !prev[button.ordinal()];
	}

	@Override
	public boolean isButtonReleased(EMouseButton button)
	{
		return !current[button.ordinal()] && prev[button.ordinal()];
	}

}
