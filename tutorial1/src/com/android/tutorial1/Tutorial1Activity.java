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

package com.android.tutorial1;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class Tutorial1Activity extends Activity implements Runnable {

  private TextView output;
  private Handler handler;
  private static Activity act;
  
  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    super.setContentView(R.layout.main);
    act = this;
    output = (TextView)super.findViewById(R.id.output);
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
	  output.setText(foo1("prova"));
  }
  
  public void button1(View v){
	  foo2();
  }
  
  public static void foo3Callback() {
	  TextView output = (TextView)act.findViewById(R.id.output);
	  output.setText("foo3Callback called back by foo2");
  }
   
  public void run() {
  //  this.handler.postDelayed(this, 2000);
  }
  
  public native String foo1(String message);
  public native void foo2();
  
  static {
      System.loadLibrary("tutorial1");
  }
}