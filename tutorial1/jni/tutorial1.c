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
#include <unistd.h>
#include <pthread.h>

#define  LOG_TAG    "tutorial1"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static JavaVM *gJavaVM;
static jobject gInterfaceObject, gDataObject;

jstring
Java_com_android_tutorial1_Tutorial1Activity_foo1( JNIEnv* env, jobject thiz, jstring message )
{
    LOGI("foo1 called!");
	return (*env)->NewStringUTF(env, "JNI call J2C performed!");
}

void
Java_com_android_tutorial1_Tutorial1Activity_foo2( JNIEnv* env, jclass thiz )
{
	LOGI("foo2 called!");
	int status;
	int isAttached = 0;
	status = (*gJavaVM)->GetEnv(gJavaVM, (void **) &env, JNI_VERSION_1_4);
	if(status < 0) {
		LOGE("callback_handler: failed to get JNI environment, assuming native thread");
		status = (*gJavaVM)->AttachCurrentThread(gJavaVM, &env, NULL);
		if(status < 0) {
			LOGE("callback_handler: failed to attach current thread");
			return;
		}
		isAttached = 1;
	}
	jclass clazz = (*env)->GetObjectClass(env, thiz);
	if (!clazz) {
		LOGE("callback_handler: failed to get object Class");
		goto failure;
	}

	jmethodID method = (*env)->GetStaticMethodID(env, clazz, "foo3Callback", "()V");
	if(!method) {
		LOGE("callback_handler: failed to get method ID");
		goto failure;
	}

	(*env)->CallStaticVoidMethod(env, clazz, method);

failure:
	if(isAttached)
		(*gJavaVM)->DetachCurrentThread(gJavaVM);
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv *env;
    gJavaVM = vm;
    LOGI("JNI_OnLoad called");
    if ( (*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("Failed to get the environment using GetEnv()");
        return -1;
    }
//    jclass localClass = env->FindClass("MyClass");
//    jclass globalClass = reinterpret_cast<jclass>(env->NewGlobalRef(localClass));
    return JNI_VERSION_1_4;
}
