/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.tutorial3;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.view.View;
import android.widget.Toast;

public class Tutorial3Activity extends Activity implements Runnable {

	static Handler handler;

	// The three following variables have been declared static to be accessed
	// from within the static runnable callback methods run() below.
	static int int0;
	static float float0;
	static String string0;

	/** Messenger for communicating with the service. */
    Messenger mService = null;

    /** Flag indicating whether we have called bind on the service. */
    boolean mBound;

    /** IntentFilter used to receive broadcast intents launched by service */
    IntentFilter receiverFilter = new IntentFilter ();

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		super.setContentView(R.layout.main);
		// Debug.startMethodTracing();
		handler = new Handler();
		receiverFilter.addAction("com.android.tutorial3.TUTORIAL_3_INTENT");
		registerReceiver(receiver, receiverFilter);
	}

	@Override
	public void onResume() {
		super.onResume();
		handler.post(this);
	}

	@Override
	protected void onStart() {
		super.onStart();
		// Binding to the service. This automatically starts the Service
        bindService(new Intent(this, CustomService.class), mConnection,
            Context.BIND_AUTO_CREATE);
	}

	@Override
	protected void onStop() {
		super.onStop();
	}

	@Override
	public void onBackPressed() {
		super.onBackPressed();
		// Unbinding from the service
        if (mBound) {
            unbindService(mConnection);
            mBound = false;
        }
        unregisterReceiver(receiver);
	}

	@Override
	public void onPause() {
		super.onPause();
		handler.removeCallbacks(this);
		// Debug.stopMethodTracing();
	}

	/* *
	 * Buttons interacts with the Service through Messenger paradigm.
	 */

	public void button0(View v) {
		if (!mBound) return;
		Message msg = Message.obtain(null, CustomService.MSG_RUN_FOO1, 0, 0);
		try {
			mService.send(msg);
		} catch (RemoteException e) {
			e.printStackTrace();
		}
	}

	public void button1(View v) {
		if (!mBound) return;
		Message msg = Message.obtain(null, CustomService.MSG_RUN_FOO2, 0, 0);
		try {
			mService.send(msg);
		} catch (RemoteException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void run() {
	}

	/**
	 * Class for interacting with the main interface of the service.
	 */
	private final ServiceConnection mConnection = new ServiceConnection() {
		@Override
		public void onServiceConnected(ComponentName className,
				IBinder service) {
			mService = new Messenger(service);
			mBound = true;
		}
		@Override
		public void onServiceDisconnected(ComponentName className) {
			mService = null;
			mBound = false;
		}
	};

	/**
	 * Broadcast receiver: catches messages sent by the Tutorial3Service
	 */
	BroadcastReceiver receiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			int running = intent.getIntExtra("CALLBACK_EXEC", 0);
			switch (running) {
				case 1:
					Toast.makeText(getApplicationContext(), "Exec. callback1",
							Toast.LENGTH_SHORT).show();
					break;
				case 2:
					Toast.makeText(getApplicationContext(), "Exec. callback2",
							Toast.LENGTH_SHORT).show();
					break;
				case 3:
					Toast.makeText(getApplicationContext(), "Exec. callback3",
							Toast.LENGTH_SHORT).show();
					break;
				case 4:
					Toast.makeText(getApplicationContext(), "Exec. callback4",
							Toast.LENGTH_SHORT).show();
					break;
			}
		}
	};
}