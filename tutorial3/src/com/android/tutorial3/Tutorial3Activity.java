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
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

public class Tutorial3Activity extends Activity implements Runnable {

	static Handler handler;

	// The three following variables have been declared static to be accessed
	// from within the static runnable callback methods run() below.
	static int int0;
	static float float0;
	static String string0;

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		super.setContentView(R.layout.main);
		// Debug.startMethodTracing();
		handler = new Handler();
	}

	@Override
	public void onResume() {
		super.onResume();
		handler.post(this);
	}

	@Override
	public void onPause() {
		super.onPause();
		handler.removeCallbacks(this);
		// Debug.stopMethodTracing();
	}

	public void button0(View v) {
		foo1();
	}

	public void button1(View v) {
		foo2();
	}

	/*
	 * Native functions to start the daemon, and stop it.
	 */

	public native void foo1();

	public native void foo2();

	public void run() {
		// handler.postDelayed(this, 2000);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.menu, menu);
		// return super.onCreateOptionsMenu(menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		Intent intent = new Intent(this, CustomService.class);

		switch (item.getItemId()) {
		case R.id.item_start_service:
			startService(intent);
			return true;
		case R.id.item_stop_service:
			stopService(intent);
			return true;
		default:
			return false;
		}
	}
}
