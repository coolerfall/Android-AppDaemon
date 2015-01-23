package com.cooler.daemon;

import android.content.Context;
import android.util.Log;

import java.io.File;
import java.io.IOException;

/**
 * Daemon: service daemon.
 *
 * @author Vincent Cheung
 * @since  Jan. 19, 2015
 */
public class Daemon {
	private static final String TAG = Daemon.class.getSimpleName();

	private static final String DAEMON_BIN_NAME = "daemon";

	public static final int INTERVAL_ONE_MINUTE = 60;
	public static final int INTERVAL_ONE_HOUR = 60 * 60;

	/** start daemon */
	private static void start(Context context, Class<?> daemonClazzName,
			int interval, String daemonFileDir) {
		String cmd = context.getDir(Command.BIN_DIR_NAME, Context.MODE_PRIVATE)
				.getAbsolutePath() + File.separator + DAEMON_BIN_NAME;

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
			Runtime.getRuntime().exec(cmdBuilder.toString()).waitFor();
		} catch (IOException | InterruptedException e) {
			Log.e(TAG, "start daemon error: " + e.getMessage());
		}
	}

	/**
	 * Run daemon process.
	 *
	 * @param context            context
	 * @param daemonServiceClazz the name of daemon service class
	 * @param interval           the interval to check
	 * @param daemonFileDir      directory of daemon file
	 */
	public static void run(final Context context,  final Class<?> daemonServiceClazz,
						final int interval, final String daemonFileDir) {
		new Thread(new Runnable() {
			@Override
			public void run() {
				Command.install(context, DAEMON_BIN_NAME);
				start(context, daemonServiceClazz, interval, daemonFileDir);
			}
		}).start();
	}

	/**
	 * Run daemon process.
	 *
	 * @param context            context
	 * @param daemonServiceClazz the name of daemon service class
	 * @param interval           the interval to check
	 */
	public static void run(final Context context,
						final Class<?> daemonServiceClazz, final int interval) {
		run(context, daemonServiceClazz, interval, null);
	}
}
