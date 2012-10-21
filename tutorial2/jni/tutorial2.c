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

#define  LOG_TAG    "tutorial2"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static JavaVM *gJavaVM;
static jobject gInterfaceObject, gDataObject;
static jclass gClass;

typedef enum {
  JNI_WRAPPER_rVOID_pVOID = 0,
  JNI_WRAPPER_rINT_pVOID = 1,
} jniWrapper_t;

typedef struct {
    const char* cbName;
    const char* cbSignature;
    jniWrapper_t jniWrapper;
    jmethodID cbMethod;
} callback_t;

/*
 * Declaration of callbacks methods, with regard to the callback_t structure:
 */
callback_t cb[] = {
  // callback1 [0]
  {
    "callback1", "()V", JNI_WRAPPER_rVOID_pVOID,
  },
  // callback2 [1]
  {
    "callback2", "()V", JNI_WRAPPER_rVOID_pVOID,
  },
};

const char method[2][10] = {"callback1", "callback2"};

jmethodID methodID[2];

int flag = 1;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv *env;
    gJavaVM = vm;
    LOGI("JNI_OnLoad called");
    if ( (*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("Failed to get the environment using GetEnv()");
        return -1;
    }
    return JNI_VERSION_1_4;
}

void
Java_com_android_tutorial2_Tutorial2Activity_init( JNIEnv* env, jobject thiz )
{
	LOGI("init called!");
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
	gClass = (jclass)(*env)->NewGlobalRef(env, clazz);
	if (!clazz) {
		LOGE("callback_handler: failed to get object Class");
		goto failure;
	}

	//The following cycle will resolve all the methodIDs declared in the cb[] array
	if(!cb)
		goto failure;

	int length = sizeof cb / sizeof cb[0];;
	while(length--) {
		LOGI("Method %d %s", length, cb[length].cbName);
		cb[length].cbMethod = (*env)->GetStaticMethodID(env, clazz, cb[length].cbName, cb[length].cbSignature);
		if(!cb[length].cbMethod) {
			LOGE("callback_handler: failed to get method ID");
			goto failure;
		}
	}

failure:
	if(isAttached)
		(*gJavaVM)->DetachCurrentThread(gJavaVM);
}

int callStaticMethodWrapper(callback_t cb) {
	LOGI("callStaticMethodWrapper called!");
	JNIEnv *env;
	int isAttached = 0;
	int status = (*gJavaVM)->GetEnv(gJavaVM, (void **) &env, JNI_VERSION_1_4);
	if(status < 0) {
		LOGE("callback_handler: failed to get JNI environment, assuming native thread");
		status = (*gJavaVM)->AttachCurrentThread(gJavaVM, &env, NULL);
		if(status < 0) {
			LOGE("callback_handler: failed to attach current thread");
			return -1;
		}
		isAttached = 1;
	}
	LOGI("Entering the switch for cb method %s with signature %s, case %d", cb.cbName, cb.cbSignature, cb.jniWrapper);
	switch (cb.jniWrapper) {
	  //
	  case JNI_WRAPPER_rVOID_pVOID:
		  (*env)->CallStaticVoidMethod(env, gClass, cb.cbMethod);
	  break;
	  case JNI_WRAPPER_rINT_pVOID:
	    (*env)->CallStaticIntMethod(env, gClass, cb.cbMethod);
	  break;
	}
	if(isAttached)
		(*gJavaVM)->DetachCurrentThread(gJavaVM);
	return 0;
}

void daemonStart() {
	LOGI("daemonStart called!");
	flag = 1;
//	while(flag) {
//	//randomize callbacks
//	}
	callStaticMethodWrapper(cb[0]);
}

void
Java_com_android_tutorial2_Tutorial2Activity_foo1( JNIEnv* env, jobject thiz)
{
    LOGI("foo1 called!");
	daemonStart();
}

void daemonStop() {
	LOGI("daemonStop called!");
	flag = 0;
}

void
Java_com_android_tutorial2_Tutorial2Activity_foo2( JNIEnv* env, jclass thiz )
{
	daemonStop();
}
