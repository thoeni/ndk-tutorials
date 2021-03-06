package com.android.tutorial3;

import android.util.Log;

public class CustomService extends Tutorial3Service {

	static final String TAG = "CustomService";
	
	@Override
	public void callback1() {		
//		super.callback1();
		Log.d(TAG, "callback1() executed.");
	}

	@Override
	public int callback2(int param0, float param1, String param2) {
		Log.d(TAG, "callback2(int, float, String) executed: int "+param0+", float "+param1+", String "+param2);
//		return super.callback2(param0, param1, param2);
		return 0;
	}

	@Override
	public void callback3(String param0) {
//		super.callback3(param0);
		Log.d(TAG, "callback3(String) executed: String "+param0);
	}

	@Override
	public float callback4(float param0) {
		Log.d(TAG, "callback4(float) executed: float "+param0);
//		return super.callback4(param0);
		return 0;
	}

}
