package org.cryo.demo;


import org.cryo.core.CsCoreLibrary;
import org.cryo.core.CsEngine;
import org.cryo.core.CsModuleConfig;
import org.cryo.core.entity.CsWorld;
import org.cryo.core.resource.CsVFS;
import org.cryo.core.resource.CsVFSConfigReader;
import org.cryo.lwjgl.window.AwtGlCanvas;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class Main
{


	private static String[] enrich(String[] args)
	{
		List<String> newArgs = new ArrayList<String>();
		boolean haveProfile = false;
		for (int i = 0; i < args.length; i++)
		{
			if (args[i].equals("--glProfile") && i + 1 < args.length)
			{
				newArgs.add(args[i]);
				newArgs.add("compat");
				i++;
				haveProfile = true;
			}
			else
			{
				newArgs.add(args[i]);
			}
		}
		if (!haveProfile)
		{
			newArgs.add("--glProfile");
			newArgs.add("compat");
		}
		return newArgs.toArray(new String[0]);
	}

	public static void main(String[] origArgs)
	{
		final String[] args = enrich(origArgs);
		SwingUtilities.invokeLater(() -> {
			CsCoreLibrary.load();

			String rootPath = "F:\\DEV\\C++\\CryoStormDemoData\\data";
			String configFileName = "vfs_java.config";

			for (int i = 0; i < args.length; i++)
			{
				if (args[i].equals("--data") && args.length > (i + 1))
				{
					rootPath = args[++i];
				}
				if (args[i].equals("--config") && args.length > (i + 1))
				{
					configFileName = args[++i];
				}
			}

			CsVFS.instance().setRootPath(rootPath);
			CsVFSConfigReader.read(rootPath, configFileName);

			// this causes on c++ side to initialize the reflection system
			// without this call no connection between java and c++ is handled
			CsEngine.get();

			AwtGlCanvas shared = null;
			{
				JFrame frame = new JFrame();
				frame.setLayout(new BorderLayout());
				AwtGlCanvas testCanvas = new AwtGlCanvas(null, () -> initializeEngine(args), Main::canvasInitialized);
				testCanvas.name = "Red";
				frame.add(testCanvas, BorderLayout.CENTER);
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				frame.setSize(1024, 768);
				frame.setLocation(-1100, 100);
				frame.setVisible(true);

				testCanvas.startRenderLoop();
				shared = testCanvas;
			}


		});
	}


	private static CsWorld world = null;

	private static void canvasInitialized(AwtGlCanvas canvas)
	{
		if (world == null)
		{
			world = new CsWorld();
			canvas.getViewport().setWorld(world);
			DemoWorld.create(world);
		}
		else
		{
			canvas.getViewport().setWorld(world);
		}
	}


	private static boolean initialized = false;

	public static void initializeEngine(String[] args)
	{
		if (initialized)
		{
			return;
		}

		CsModuleConfig moduleConfig = new CsModuleConfig();
		if (!moduleConfig.loadModuleConfig())
		{
			System.out.println("Failed to load module config");
			return;
		}

		CsEngine.get().initializeEngine(args, moduleConfig);

		initialized = true;

	}

}
