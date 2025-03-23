package org.cryo.lwjgl.window.input;

import org.cryo.core.input.EKey;
import org.cryo.core.input.CsKeyboardJava;

import java.awt.event.KeyEvent;

public class AwtGlCanvasKeyboard extends CsKeyboardJava
{

	private boolean[] primed = new boolean[EKey.values().length];
	private boolean[] current = new boolean[EKey.values().length];
	private boolean[] prev = new boolean[EKey.values().length];

	public AwtGlCanvasKeyboard()
	{
	}

	public void update()
	{
		System.arraycopy(current, 0, prev, 0, EKey.values().length);
		System.arraycopy(primed, 0, current, 0, EKey.values().length);
	}


	public void primeKey(KeyEvent key, boolean down)
	{
		EKey cryo = mapSwingToCryo(key);
		if (cryo != null)
		{
			primed[cryo.ordinal()] = down;
		}
	}

	@Override
	public boolean isKeyDown(EKey key)
	{
		return current[key.ordinal()];
	}

	@Override
	public boolean isKeyUp(EKey key)
	{
		return !current[key.ordinal()];
	}

	@Override
	public boolean isKeyPressed(EKey key)
	{
		return current[key.ordinal()] && !prev[key.ordinal()];
	}

	@Override
	public boolean isKeyReleased(EKey key)
	{
		return !current[key.ordinal()] && prev[key.ordinal()];
	}


	private EKey mapSwingToCryo (KeyEvent e)
	{
		boolean left = e.getKeyLocation() == KeyEvent.KEY_LOCATION_LEFT;
		boolean right = e.getKeyLocation() == KeyEvent.KEY_LOCATION_RIGHT;
		boolean numPad = e.getKeyLocation() == KeyEvent.KEY_LOCATION_NUMPAD;

		return switch (e.getKeyCode()) {
			case KeyEvent.VK_ESCAPE -> EKey.KEY_Escape;
			case KeyEvent.VK_A -> EKey.KEY_A;
			case KeyEvent.VK_B -> EKey.KEY_B;
			case KeyEvent.VK_C -> EKey.KEY_C;
			case KeyEvent.VK_D -> EKey.KEY_D;
			case KeyEvent.VK_E -> EKey.KEY_E;
			case KeyEvent.VK_F -> EKey.KEY_F;
			case KeyEvent.VK_G -> EKey.KEY_G;
			case KeyEvent.VK_H -> EKey.KEY_H;
			case KeyEvent.VK_I -> EKey.KEY_I;
			case KeyEvent.VK_J -> EKey.KEY_J;
			case KeyEvent.VK_K -> EKey.KEY_K;
			case KeyEvent.VK_L -> EKey.KEY_L;
			case KeyEvent.VK_M -> EKey.KEY_M;
			case KeyEvent.VK_N -> EKey.KEY_N;
			case KeyEvent.VK_O -> EKey.KEY_O;
			case KeyEvent.VK_P -> EKey.KEY_P;
			case KeyEvent.VK_Q -> EKey.KEY_Q;
			case KeyEvent.VK_R -> EKey.KEY_R;
			case KeyEvent.VK_S -> EKey.KEY_S;
			case KeyEvent.VK_T -> EKey.KEY_T;
			case KeyEvent.VK_U -> EKey.KEY_U;
			case KeyEvent.VK_V -> EKey.KEY_V;
			case KeyEvent.VK_W -> EKey.KEY_W;
			case KeyEvent.VK_X -> EKey.KEY_X;
			case KeyEvent.VK_Y -> EKey.KEY_Y;
			case KeyEvent.VK_Z -> EKey.KEY_Z;
			case KeyEvent.VK_1 -> EKey.KEY_1;
			case KeyEvent.VK_2 -> EKey.KEY_2;
			case KeyEvent.VK_3 -> EKey.KEY_3;
			case KeyEvent.VK_4 -> EKey.KEY_4;
			case KeyEvent.VK_5 -> EKey.KEY_5;
			case KeyEvent.VK_6 -> EKey.KEY_6;
			case KeyEvent.VK_7 -> EKey.KEY_7;
			case KeyEvent.VK_8 -> EKey.KEY_8;
			case KeyEvent.VK_9 -> EKey.KEY_9;
			case KeyEvent.VK_0 -> EKey.KEY_0;

			case KeyEvent.VK_SHIFT -> mapLeftRight(EKey.KEY_LEFT_SHIFT, EKey.KEY_RIGHT_SHIFT, left, right);
			case KeyEvent.VK_ALT -> mapLeftRight(EKey.KEY_LEFT_ALT, EKey.KEY_RIGHT_ALT, left, right);
			case KeyEvent.VK_CONTROL -> mapLeftRight(EKey.KEY_LEFT_CTRL, EKey.KEY_RIGHT_CTRL, left, right);
			case KeyEvent.VK_WINDOWS -> mapLeftRight(EKey.KEY_LEFT_WIN, EKey.KEY_RIGHT_WIN, left, right);

			case KeyEvent.VK_SPACE -> EKey.KEY_SPACE;

			case KeyEvent.VK_LEFT -> EKey.KEY_LEFT;
			case KeyEvent.VK_RIGHT -> EKey.KEY_RIGHT;
			case KeyEvent.VK_UP -> EKey.KEY_UP;
			case KeyEvent.VK_DOWN -> EKey.KEY_DOWN;
			default -> null;
		};
	}

	private EKey mapLeftRight(EKey leftKey, EKey rightKey, boolean isLeft, boolean isRight)
	{
		if (isLeft)
		{
			return leftKey;
		}
		if (isRight)
		{
			return rightKey;
		}
		return null;
	}
}
