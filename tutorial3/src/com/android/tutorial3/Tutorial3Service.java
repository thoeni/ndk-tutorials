package com.android.tutorial3;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public class Tutorial3Service extends Service {
	static final String TAG = "Tutorial3Service";
	 //Testare questo:
	public int recipe;
	
	@Override
	public void onCreate() {
		super.onCreate();
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
	
	public void callback1() {
	}
	
	public int callback2(int param0, float param1, String param2) {
		return 0;
	}
	
	public void callback3(String param0) {
	}
	
	public float callback4(final float param0) {
		return param0;
	}

	//Provare a inizializzare variabili qui sotto..
	static {
		System.loadLibrary("tutorial3");
	}

}
