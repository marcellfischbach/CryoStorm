package org.cryo.core;

import java.io.File;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;

public class CsCoreLibrary
{

	private static final String LIBRARY_PATH = System.getProperty("user.dir");

	private static final String[] LIBRARY_NAMES = new String[]{
			"csAssimpLoader",
			"csBullet",
			"csCore",
			"csCryoFile",
			"csGame",
			"csImgLoader",
			"csOpenGL",
			"csXml"
	};


	public static void load()
	{
		try
		{
			extractLibraries();

			Runtime.getRuntime().addShutdownHook(new Thread(() -> {
				removeLibraries();
				System.out.println("Custom shutdown hook is running...");
				// Add your cleanup code here
			}));

			System.load(LIBRARY_PATH + "/" + System.mapLibraryName("csCore"));
		}
		catch (UnsatisfiedLinkError le)
		{
			System.err.println("Unable to load csCore: " + System.mapLibraryName("csCore"));
			throw le;

		}
		catch (Exception e)
		{
			System.err.println("Unable to load csCore: " + System.mapLibraryName("csCore"));
			throw new RuntimeException(e);
		}
	}

	private static void extractLibraries() throws Exception
	{
		File path = new File(LIBRARY_PATH);
		Files.createDirectories(path.toPath());
		for (String libraryName : LIBRARY_NAMES)
		{
			extractLibrary(libraryName, path);

		}
	}


	private static void extractLibrary(String libName, File path) throws Exception
	{
		String nativeLibName = System.mapLibraryName(libName);
		InputStream dllStream = CsCoreLibrary.class.getResourceAsStream("/natives/" + nativeLibName);

		System.out.println("Extract '" + nativeLibName + "' to " + path);

		File tmpDllFile = new File(path + "/" + nativeLibName);
		Path tempDll = tmpDllFile.toPath();
		Files.copy(dllStream, tempDll, StandardCopyOption.REPLACE_EXISTING);
		dllStream.close();


		tmpDllFile.deleteOnExit();

	}


	private static void removeLibraries()
	{
		File path = new File(LIBRARY_PATH);
		for (String libraryName : LIBRARY_NAMES)
		{
			removeLibrary(libraryName, path);
		}
	}

	private static void removeLibrary(String libName, File path)
	{
		try
		{
			String nativeLibName = System.mapLibraryName(libName);
			File tmpDllFile = new File(path + "/" + nativeLibName);

			tmpDllFile.deleteOnExit();
		}
		catch (Throwable th)
		{
			th.printStackTrace();
		}
	}

	private static void markForDeletion(File file)
	{
		if (file.isDirectory())
		{
			// Recursively mark all files and subdirectories
			for (File child : file.listFiles())
			{
				markForDeletion(child);
			}
		}
		// Mark the file or directory for deletion on exit
		file.deleteOnExit();
	}

	private static void deleteDirectory(File file)
	{
		if (file.isDirectory())
		{
			// Recursively delete all files and subdirectories
			for (File child : file.listFiles())
			{
				deleteDirectory(child);
			}
		}
		// Delete the file or empty directory
		file.delete();
	}

	private CsCoreLibrary()
	{

	}

}
