/*
 * Copyright (C) 2009 The Android Open Source Project
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

// OpenGL ES 2.0 code
#include <jni.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  LOG_TAG    "tutorial1"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

jstring Java_com_android_tutorial1_Tutorial1Activity_foo1(JNIEnv* env,
		jobject thiz, jstring message) {
	//To print out a char* we have to convert the jstring to char*
	const char *nativeString = (*env)->GetStringUTFChars(env, message, 0);
	LOGI("foo1 called! Input parameter: %s", nativeString);
	//Then we have to release the memory allocated for the string
	(*env)->ReleaseStringUTFChars(env, message, nativeString);
	return (*env)->NewStringUTF(env, "JNI call J2C performed!");
}

void Java_com_android_tutorial1_Tutorial1Activity_foo2(JNIEnv* env,
		jobject thiz) {
	LOGI("foo2 called!");
	//Get class from the calling object
	jclass clazz = (*env)->GetObjectClass(env, thiz);
	if (!clazz) {
		LOGE("callback_handler: failed to get object Class");
		goto failure;
	}
	//Get the methodID from the class which the calling object belongs
	jmethodID method = (*env)->GetMethodID(env, clazz, "foo3Callback", "()V");
	if (!method) {
		LOGE("callback_handler: failed to get method ID");
		goto failure;
	}
	//Call the method on the calling object, defined by the methodID
	(*env)->CallVoidMethod(env, thiz, method);

	failure: return;
}
