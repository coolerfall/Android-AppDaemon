package com.coolerfall.ui;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;

import com.coolerfall.daemon.sample.R;
import com.coolerfall.service.DaemonService;

public class MainActivity extends ActionBarActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		startService(new Intent(this, DaemonService.class));
	}
}
