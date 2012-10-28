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

package com.android.tutorial2;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.TextView;

public class Tutorial2Activity extends Activity implements Runnable {

	private TextView output;
	private Handler handler;

	// The three following variables have been declared static to be accessed
	// from within the static runnable callback methods run() below.
	static int int0;
	static float float0;
	static String string0;

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		super.setContentView(R.layout.main);
		output = (TextView) findViewById(R.id.output);
		init();
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
		// TODO: Check this
		handler.removeCallbacks(this);
	}

	public void button0(View v) {
		foo1();
	}

	public void button1(View v) {
		foo2();
	}

	/*
	 * Callback methods are declared as static, because we want a native
	 * function to call them asynchronously, with no reference to the current
	 * object. Therefore our native functions will rely only on the Activity
	 * class, and on its static methods.
	 * 
	 * For each callback method, we declare a (static, for the same reason as
	 * before) Runnable object, that will be able to interact with the User
	 * Interface from within the thread call. If we don't pass through this
	 * mechanism, an exception will be thrown, because by default only the
	 * generating thread can access to the View object, and not other threads
	 * (as the *randomCaller wants to do, from native code.
	 */

	public void callback1() {
		System.out.println("callback1 called");
		handler.post(callback1Thread);
	}

	Runnable callback1Thread = new Runnable() {
		@Override
		public void run() {
			output.setText("callback 1, no params");
		}
	};

	public int callback2(int param0, float param1, String param2) {
		System.out.println("callback2 called, params are: " + param0 + " "
				+ param1 + " " + param2);
		int0 = param0;
		float0 = param1;
		string0 = param2;
		handler.post(callback2Thread);
		return 0;
	}

	Runnable callback2Thread = new Runnable() {
		@Override
		public void run() {
			output.setText("callback 2, params are: " + int0 + ", " + float0
					+ ", " + string0);
		}
	};

	public void callback3(String param0) {
		System.out.println("callback 3, param is: " + param0);
		string0 = param0;
		handler.post(callback3Thread);
	}

	Runnable callback3Thread = new Runnable() {
		@Override
		public void run() {
			output.setText("callback 3, param is: " + string0);
		}
	};

	public float callback4(float param0) {
		System.out.println("callback 4, param is: " + param0);
		float0 = param0;
		handler.post(callback4Thread);
		return param0;
	}

	Runnable callback4Thread = new Runnable() {
		@Override
		public void run() {
			output.setText("callback 4, param is: " + float0);
		}
	};

	public void run() {

	}

	public native void init();

	public native void foo1();

	public native void foo2();

	static {
		System.loadLibrary("tutorial2");
	}
}
