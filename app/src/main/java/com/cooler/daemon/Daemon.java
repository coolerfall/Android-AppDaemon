package com.cooler.daemon;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Build;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * Daemon: service daemon.
 *
 * @author Vincent Cheung
 * @since  Jan. 19, 2015
 */
public class Daemon {
	private static final String TAG = Daemon.class.getSimpleName();

	private static final String BIN_DIR_NAME = "bin";
	private static final String DAEMON_BIN_NAME = "daemon";

	public static final int INTERVAL_ONE_MINUTE = 60;
	public static final int INTERVAL_ONE_HOUR = 60 * 60;

	/** copy file */
	private static void copyFile(File file, InputStream is, String mode)
			throws IOException, InterruptedException {
		final String abspath = file.getAbsolutePath();
		final FileOutputStream out = new FileOutputStream(file);
		byte buf[] = new byte[1024];
		int len;
		while ((len = is.read(buf)) > 0) {
			out.write(buf, 0, len);
		}

		out.close();
		is.close();

		Runtime.getRuntime().exec("chmod " + mode + " " + abspath).waitFor();
	}

	/** copy files in raw into destination file */
	private static void copyRawFile(Context context, int resid, File file, String mode)
			throws IOException, InterruptedException {
		final InputStream is = context.getResources().openRawResource(resid);
		copyFile(file, is, mode);
	}

	/** copy file in assets into destination file */
	private static void copyAssets(Context context, String assetsFilename, File file, String mode)
			throws IOException, InterruptedException {
		AssetManager manager = context.getAssets();
		final InputStream is = manager.open(assetsFilename);
		copyFile(file, is, mode);
	}

	/** install binary */
	private static boolean installBinary(Context context, String filename) {
		try {
			File f = new File(context.getDir(BIN_DIR_NAME, Context.MODE_PRIVATE), filename);
			if (f.exists()) {
				Log.d(TAG, "binary has existed");
				return false;
			}

//			copyRawFile(context, resId, f, "0755");
			copyAssets(context, DAEMON_BIN_NAME, f, "0755");
			return true;
		} catch (Exception e) {
			Log.e(TAG, "installBinary failed: " + e.getMessage());
			return false;
		}
	}

	/** install all the binary in raw */
	@SuppressWarnings("deprecation")
	private static boolean install(Context context) {
		String abi = Build.CPU_ABI;
		return abi.startsWith("arm") && installBinary(context, DAEMON_BIN_NAME);
	}

	/** start daemon */
	private static void start(Context context, Class<?> daemonClazzName,
			int interval, String daemonFileDir) {
		String cmd = context.getDir(BIN_DIR_NAME, Context.MODE_PRIVATE)
				.getAbsolutePath() + File.separator + "daemon";

		/* create the command string */
		StringBuilder cmdBuilder = new StringBuilder();
		cmdBuilder.append(cmd);
		cmdBuilder.append(" -p ");
		cmdBuilder.append(context.getPackageName());
		cmdBuilder.append(" -s ");
		cmdBuilder.append(daemonClazzName.getName());
		cmdBuilder.append(" -t ");
		cmdBuilder.append(interval);

		if (daemonFileDir != null) {
			/* to check if the daemon file dir existed */
			File dir = new File(daemonFileDir);
			if (!dir.exists()) {
				dir.mkdirs();
			}

			cmdBuilder.append(" -d ");
			cmdBuilder.append(daemonFileDir);
		}

		try {
			Runtime.getRuntime().exec(cmdBuilder.toString());
		} catch (IOException e) {
			Log.e(TAG, "start daemon error: " + e.getMessage());
		}
	}

	/**
	 * Run daemon process.
	 *
	 * @param context         context
	 * @param daemonClazzName the name of daemon service class
	 * @param interval        the interval to check
	 * @param daemonFileDir   directory of daemon file
	 */
	public static void run(final Context context,  final Class<?> daemonClazzName,
						final int interval, final String daemonFileDir) {
		new Thread(new Runnable() {
			@Override
			public void run() {
				install(context);
				start(context, daemonClazzName, interval, daemonFileDir);
			}
		}).start();
	}

	/**
	 * Run daemon process.
	 *
	 * @param context         context
	 * @param daemonClazzName the name of daemon service class
	 * @param interval        the interval to check
	 */
	public static void run(final Context context,
						final Class<?> daemonClazzName, final int interval) {
		run(context, daemonClazzName, interval, null);
	}
}
