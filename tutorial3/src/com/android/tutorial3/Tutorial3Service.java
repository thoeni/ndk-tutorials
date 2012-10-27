package com.android.tutorial3;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public abstract class Tutorial3Service extends Service {
	static final String TAG = "Tutorial3Service";
	 //Testare questo:
	
	static int recipe = 0;
	
	static void setRecipe(int recipes){
		recipe = recipes;
	}
	
	@Override
	public void onCreate() {
		super.onCreate();
		init(recipe);
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

	public native void init(int recipe);

	public native void foo1();

	public native void foo2();
	
	abstract void callback1();
	
	abstract int callback2(int param0, float param1, String param2);
	
	abstract void callback3(String param0);
	
	abstract float callback4(final float param0);

	//Provare a inizializzare variabili qui sotto..
	static {
		System.loadLibrary("tutorial3");
	}

}
