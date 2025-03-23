package org.cryo.lwjgl.window;

import org.cryo.core.CsObjectRegistry;
import org.cryo.core.Viewport;
import org.cryo.core.graphics.ERenderPipeline;
import org.cryo.core.graphics.IDevice;
import org.cryo.core.input.IInputSystem;
import org.cryo.core.window.IWindow;
import org.cryo.lwjgl.window.input.AwtGlCanvasInputSystem;
import org.lwjgl.opengl.awt.AWTGLCanvas;
import org.lwjgl.opengl.awt.GLData;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class AwtGlCanvas extends AWTGLCanvas
{

	public String name;

	private final IWindow window = new WindowLwjgl(this);

	private final AwtGlCanvasInputSystem inputSystem = new AwtGlCanvasInputSystem();

	private Viewport viewport;

	private final RenderThread renderThread = new RenderThread();


	public interface InitEngineCallback
	{

		void initEngine();

	}

	public interface CanvasInitializedCallback
	{

		void canvasInitialized(AwtGlCanvas canvas);

	}

	public interface AfterRenderCallback
	{

		void afterRender(AwtGlCanvas canvas);

	}

	private final InitEngineCallback initEngineCallback;
	private final CanvasInitializedCallback canvasInitializedCallback;
	private AfterRenderCallback afterRenderCallback = null;


	public AwtGlCanvas(AWTGLCanvas sharedCanvas,
	                   InitEngineCallback initEngineCallback,
	                   CanvasInitializedCallback canvasInitializedCallback)
	{
		super(glData(sharedCanvas));


		this.initEngineCallback = initEngineCallback;
		this.canvasInitializedCallback = canvasInitializedCallback;
		this.renderThread.start();

		initialize();


		initializeInputSystem();


	}


	public void setAfterRenderCallback(AfterRenderCallback afterRenderCallback)
	{
		this.afterRenderCallback = afterRenderCallback;
	}

	private boolean renderLoopActive = false;

	public void startRenderLoop()
	{
		this.renderThread.issueRendering();
	}


	public void stopRenderLoop()
	{
		this.renderLoopActive = false;
	}

	public IWindow getWindow()
	{
		return this.window;
	}

	public Viewport getViewport()
	{
		return this.viewport;
	}

	@Override
	public void initGL()
	{

		this.initEngineCallback.initEngine();


		this.viewport = new Viewport();
		this.viewport.setWindow(this.window);
		this.viewport.setDevice(CsObjectRegistry.get(IDevice.class));
		this.viewport.setRenderPipeline(ERenderPipeline.DEFERRED);

		this.canvasInitializedCallback.canvasInitialized(this);


	}

	public void initialize()
	{
		SwingUtilities.invokeLater(() -> {
			beforeRender();
			try
			{
				initGL();
				initCalled = true;
			}
			finally
			{
				afterRender();
			}
		});
	}

	protected void afterRender()
	{
//        platformCanvas.makeCurrent(0L);
		try
		{
			platformCanvas.unlock(); // <- MUST unlock on Linux
		}
		catch (AWTException e)
		{
			throw new RuntimeException("Failed to unlock Canvas", e);
		}

	}

	@Override
	public void paintGL()
	{

		// this frame performs the updates on the input system of this frames window
		CsObjectRegistry.register(IInputSystem.class, inputSystem);

		this.inputSystem.getKeyboard().update();
		this.inputSystem.getMouse().update();

		viewport.processFrame(null);

		if (this.afterRenderCallback != null)
		{
			this.afterRenderCallback.afterRender(this);
		}
		swapBuffers();

		this.renderThread.issueRendering();
	}


	private static GLData glData(AWTGLCanvas sharedCanvas)
	{
		GLData data = new GLData();
		data.majorVersion = 4;
		data.minorVersion = 4;
		data.api = GLData.API.GL;
		data.profile = GLData.Profile.COMPATIBILITY;
		data.swapInterval = 0;
		data.shareContext = null; //sharedCanvas;
		data.robustness = true;
		data.forwardCompatible = false;
		System.out.println("Shared context: " + sharedCanvas);
		return data;
	}


	private void initializeInputSystem()
	{
		addMouseMotionListener(new MouseMotionListener()
		{
			@Override
			public void mouseDragged(MouseEvent e)
			{
				AwtGlCanvas.this.mouseMoved(e);
			}

			@Override
			public void mouseMoved(MouseEvent e)
			{
				AwtGlCanvas.this.mouseMoved(e);
			}
		});

		addMouseListener(new MouseAdapter()
		{
			@Override
			public void mousePressed(MouseEvent e)
			{
				AwtGlCanvas.this.mousePressed(e);
			}

			@Override
			public void mouseReleased(MouseEvent e)
			{
				AwtGlCanvas.this.mouseReleased(e);
			}
		});

		addKeyListener(new KeyAdapter()
		{
			@Override
			public void keyPressed(KeyEvent e)
			{
				AwtGlCanvas.this.keyPressed(e);
			}

			@Override
			public void keyReleased(KeyEvent e)
			{
				AwtGlCanvas.this.keyReleased(e);
			}
		});
	}

	private void mouseMoved(MouseEvent e)
	{
		inputSystem.getMouse().primePosition(e.getX(), e.getY());
	}

	private void mousePressed(MouseEvent e)
	{
		inputSystem.getMouse().primeButton(e.getButton(), true);
	}

	private void mouseReleased(MouseEvent e)
	{
		inputSystem.getMouse().primeButton(e.getButton(), false);
	}

	private void keyPressed(KeyEvent e)
	{
		inputSystem.getKeyboard().primeKey(e, true);
	}

	private void keyReleased(KeyEvent e)
	{
		inputSystem.getKeyboard().primeKey(e, false);
	}


	private class RenderThread extends Thread
	{

		private boolean running = true;

		private boolean issueRendering;


		public void issueRendering ()
		{
			this.issueRendering = true;
		}

		@Override
		public void run()
		{
			while (running)
			{
				try
				{
					Thread.sleep(1);
					if (issueRendering)
					{
						issueRendering = false;
						SwingUtilities.invokeLater(AwtGlCanvas.this::render);
					}
				}
				catch (Exception e)
				{

				}
			}
		}

	}
}

