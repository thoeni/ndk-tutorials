package com.android.tutorial3;

import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.util.Log;
import android.widget.Toast;

public abstract class Tutorial3Service extends Service {
	static final String TAG = "Tutorial3Service";

	//******* Available messages to the Service *******
	static final int MSG_RUN_FOO1= 1;
	static final int MSG_RUN_FOO2 = 2;
	//*************************************************

	/*
	 * recipe is 0 by default
	 * The CustomService developer has to use the setRecipe in case he wants
	 * to define another recipe to initialize the library.
	 */
	int recipe = 0;

	protected Tutorial3Service(int recipe) {
		this.recipe = recipe;
	}

	@Override
	public void onCreate() {
		Log.d(TAG, "onCreated");
		super.onCreate();
		init("carbonara");
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		Log.d(TAG, "onStarted");
		return super.onStartCommand(intent, flags, startId);
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		Log.d(TAG, "onDestroy");
	}

	@Override
	public IBinder onBind(Intent intent) {
		return mMessenger.getBinder();
	}

	/**
	 * Handler of incoming messages from clients.
	 */
	class IncomingHandler extends Handler {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MSG_RUN_FOO1:
				Toast.makeText(getApplicationContext(), "Calling foo1()!",
						Toast.LENGTH_SHORT).show();
				foo1();
				break;
			case MSG_RUN_FOO2:
				Toast.makeText(getApplicationContext(), "Calling foo2()!",
						Toast.LENGTH_SHORT).show();
				foo2();
				break;
			default:
				super.handleMessage(msg);
			}
		}
	}

	/* *
	 * Instantiate the target - to be sent to clients - to communicate with
	 * this instance of Service
	 */
	final Messenger mMessenger = new Messenger(new IncomingHandler());

	/*
	 * init(String recipe): initializes the environment, and sets the recipe
	 * foo1(): starts the sample daemon
	 * foo2(): stops the sample daemon
	 */

	public native void init(String recipe);

	public native void foo1();

	public native void foo2();

	/*
	 * Callback defined as abstract, do that the CustomService developer
	 * is forced to define their implementation.
	 */

	abstract void callback1();

	abstract int callback2(int param0, float param1, String param2);

	abstract void callback3(String param0);

	abstract float callback4(float param0);

	public void _callback1() {
		callback1();
		Log.i(TAG, "_callback1 called: broadcasting this information.");
		Intent intent = new Intent("com.android.tutorial3.TUTORIAL_3_INTENT");
		intent.putExtra("CALLBACK_EXEC",1);
		sendBroadcast(intent);
	}

	public int _callback2 (int param0, float param1, String param2) {
		Log.i(TAG, "_callback2 called: broadcasting this information.");
		Intent intent = new Intent("com.android.tutorial3.TUTORIAL_3_INTENT");
		intent.putExtra("CALLBACK_EXEC",2);
		sendBroadcast(intent);
		return callback2(param0, param1, param2);
	}

	public void _callback3(String param0) {
		Log.i(TAG, "_callback3 called: broadcasting this information.");
		callback3(param0);
		Intent intent = new Intent("com.android.tutorial3.TUTORIAL_3_INTENT");
		intent.putExtra("CALLBACK_EXEC",3);
		sendBroadcast(intent);
	}

	public float _callback4(float param0) {
		Log.i(TAG, "_callback4 called: broadcasting this information.");
		Intent intent = new Intent("com.android.tutorial3.TUTORIAL_3_INTENT");
		intent.putExtra("CALLBACK_EXEC",4);
		sendBroadcast(intent);
		return callback4(param0);
	}

	/*
	 * Load the native library
	 */
	static {
		System.loadLibrary("tutorial3");
	}

}
