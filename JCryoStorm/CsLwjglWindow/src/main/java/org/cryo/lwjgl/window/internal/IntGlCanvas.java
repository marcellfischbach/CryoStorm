package org.cryo.lwjgl.window.internal;

import org.lwjgl.awthacks.NonClearGraphics;
import org.lwjgl.awthacks.NonClearGraphics2D;
import org.lwjgl.opengl.awt.*;
import org.lwjgl.system.Platform;

import java.awt.*;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.geom.AffineTransform;
import java.util.concurrent.Callable;

public abstract class IntGlCanvas extends Canvas
{

	private static final long serialVersionUID = 1L;
	protected PlatformGLCanvas platformCanvas;
	protected long context;
	protected final GLData data;
	protected final GLData effective;
	protected boolean initCalled;
	private int framebufferWidth;
	private int framebufferHeight;
	private final ComponentListener listener;

	private static PlatformGLCanvas createPlatformCanvas()
	{
		switch (Platform.get())
		{
			case WINDOWS:
				return new PlatformWin32GLCanvas();
			case LINUX:
				return new PlatformLinuxGLCanvas();
			case MACOSX:
				return new PlatformMacOSXGLCanvas();
			default:
				throw new UnsupportedOperationException("Platform " + Platform.get() + " not yet supported");
		}
	}

	public void removeNotify()
	{
		super.removeNotify();
		this.context = 0L;
		this.initCalled = false;
		this.disposeCanvas();
	}

	public synchronized void addComponentListener(ComponentListener l)
	{
		super.addComponentListener(l);
	}

	public void disposeCanvas()
	{
		this.platformCanvas.dispose();
	}

	protected IntGlCanvas(GLData data)
	{
		this.platformCanvas = createPlatformCanvas();
		this.effective = new GLData();
		this.listener = new ComponentAdapter()
		{
			public void componentResized(ComponentEvent e)
			{
				AffineTransform t = IntGlCanvas.this.getGraphicsConfiguration().getDefaultTransform();
				float sx = (float) t.getScaleX();
				float sy = (float) t.getScaleY();
				IntGlCanvas.this.framebufferWidth = (int) ((float) IntGlCanvas.this.getWidth() * sx);
				IntGlCanvas.this.framebufferHeight = (int) ((float) IntGlCanvas.this.getHeight() * sy);
			}
		};
		this.data = data;
		this.addComponentListener(this.listener);
	}

	protected IntGlCanvas()
	{
		this(new GLData());
	}

	protected void beforeRender()
	{
		AWTException e;
		if (this.context == 0L)
		{
			try
			{
				this.context = this.platformCanvas.create(this, this.data, this.effective);
			}
			catch (AWTException var3)
			{
				e = var3;
				throw new RuntimeException("Exception while creating the OpenGL context", e);
			}
		}

		try
		{
			this.platformCanvas.lock();
		}
		catch (AWTException var2)
		{
			e = var2;
			throw new RuntimeException("Failed to lock Canvas", e);
		}

		this.platformCanvas.makeCurrent(this.context);
	}

	protected void afterRender()
	{
		this.platformCanvas.makeCurrent(0L);

		try
		{
			this.platformCanvas.unlock();
		}
		catch (AWTException var2)
		{
			AWTException e = var2;
			throw new RuntimeException("Failed to unlock Canvas", e);
		}
	}

	public <T> T executeInContext(Callable<T> callable) throws Exception
	{
		this.beforeRender();

		T var2;
		try
		{
			var2 = callable.call();
		}
		finally
		{
			this.afterRender();
		}

		return var2;
	}

	public void runInContext(Runnable runnable)
	{
		this.beforeRender();

		try
		{
			runnable.run();
		}
		finally
		{
			this.afterRender();
		}

	}

	public void render()
	{
		this.beforeRender();

		try
		{
			if (!this.initCalled)
			{
				this.initGL();
				this.initCalled = true;
			}

			this.paintGL();
		}
		finally
		{
			this.afterRender();
		}

	}

	public abstract void initGL();

	public abstract void paintGL();

	public int getFramebufferWidth()
	{
		return this.framebufferWidth;
	}

	public int getFramebufferHeight()
	{
		return this.framebufferHeight;
	}

	public final void swapBuffers()
	{
		this.platformCanvas.swapBuffers();
	}

	public Graphics getGraphics()
	{
		Graphics graphics = super.getGraphics();
		return (Graphics) (graphics instanceof Graphics2D ? new NonClearGraphics2D((Graphics2D) graphics) : new NonClearGraphics(graphics));
	}

}
