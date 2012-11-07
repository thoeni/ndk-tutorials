package com.android.tutorial3;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

public abstract class Tutorial3Service extends Service {
	static final String TAG = "Tutorial3Service";

	/*
	 * recipe is 0 by default
	 * The CustomService developer has to use the setRecipe in case he wants
	 * to define another recipe to initialize the library.
	 */
	int recipe = 0;
//	static void setRecipe(int recipes){
//		recipe = recipes;
//	}
//
	protected Tutorial3Service(int recipe) {
		this.recipe = recipe;
	}

	@Override
	public void onCreate() {
		Log.d(TAG, "onCreated");
		super.onCreate();
		init(recipe);
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

	/*
	 * init(int recipe): initializes the environment, and sets the recipe
	 * foo1(): starts the sample daemon
	 * foo2(): stops the sample daemon
	 */

	public native void init(int recipe);

	public native void foo1();

	public native void foo2();

	/*
	 * Callback defined as abstract, do that the CustomService developer
	 * is forced to define their implementation.
	 */

	abstract void callback1();

	abstract int callback2(int param0, float param1, String param2);

	abstract void callback3(String param0);

	abstract float callback4(final float param0);

	/*
	 * Load the native library
	 */
	static {
		System.loadLibrary("tutorial3");
	}

}
