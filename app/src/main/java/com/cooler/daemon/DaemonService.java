package com.cooler.daemon;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

public class DaemonService extends Service {
	@Override
	public void onCreate() {
		super.onCreate();
		Daemon.run(this, DaemonService.class, Daemon.INTERVAL_ONE_MINUTE * 2);
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}
}
