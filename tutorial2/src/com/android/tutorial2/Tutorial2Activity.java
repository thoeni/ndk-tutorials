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
import com.android.tutorial2.Param;

public class Tutorial2Activity extends Activity implements Runnable {

  private TextView output;
  private Handler handler;
  private static Activity act;
  
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    super.setContentView(R.layout.main);
    act = this;
    init();
    this.handler = new Handler();
  }

  @Override
  public void onResume() {
    super.onResume();
    this.handler.post(this);
  }

  @Override
  public void onPause() {
    super.onPause();
    //TODO: Check this
    this.handler.removeCallbacks(this);
  }

  public void button0(View v){
	  foo1();
  }
  
  public void button1(View v){
	  foo2();
  }
  
  public static void callback1() {
	  TextView output = (TextView)act.findViewById(R.id.output);
	  output.setText("callback1 called");
  }
  
  public static void callback2(int param) {
	  TextView output = (TextView)act.findViewById(R.id.output);
	  output.setText("callback2 called, integer param: "+param);
  }
  
  public static void callback3(int p0, int p1) {
	  TextView output = (TextView)act.findViewById(R.id.output);
	  output.setText("callback3 called, integer param: "+p0+", "+p1);
  }
  
  public static void callback4(int[] inta) {
	  TextView output = (TextView)act.findViewById(R.id.output);
	  output.setText("callback4 called, integer param: "+inta[0]+", "+inta[1]);
  }
  
  public static void callbackObject(Param parameters) {
	  TextView output = (TextView)act.findViewById(R.id.output);
	  output.setText("callbackObj: int: "+parameters.getiParams()[0]+", "+parameters.getiParams()[1]+" float: "+parameters.getfParams()[0]+" "+parameters.getfParams()[1]);
//	  float[] fParams = (float[])parameters[1];
//	  String[] sParams = (String[])parameters[2];
  }
  
  public void run() {
  //  this.handler.postDelayed(this, 2000);
  }
  
  public native void init();
  public native void foo1();
  public native void foo2();
  
  static {
      System.loadLibrary("tutorial2");
  }
}
