package com.android.tutorial2;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class MyService extends Service {
	static final String TAG = "MyService";
	private static MyService sInstance;
	
	public static MyService getInstance() {
		return sInstance;
	}
	
	@Override
	public void onCreate() {
		super.onCreate();
		// sInstance: to be used to communicate with Activity? Find out how, or
		// alternative methods.
		sInstance = this;
		init();
		Log.d(TAG, "onCreated");
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
		return null;
	}

	public native void init();

	public native void foo1();

	public native void foo2();
	
	public static void callback1() {
		System.out.println("callback1 called");
	}
	
	public static int callback2(int param0, float param1, String param2) {
		System.out.println("callback2 called, params are: " + param0 + " "+ param1 + " " + param2);
		return 0;
	}
	
	public static void callback3(String param0) {
		System.out.println("callback 3, param is: " + param0);
	}
	
	public static float callback4(final float param0) {
		System.out.println("callback 4, param is: " + param0);
		return param0;
	}

	static {
		System.loadLibrary("tutorial2");
	}

}
