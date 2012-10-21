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
  JNI_WRAPPER_rVOID_pINT = 1,
  JNI_WRAPPER_rVOID_pINT_pINT = 2,
} jniWrapper_t;

typedef struct {
	int p_int;
	float p_float;
	char* p_string;
} params_t;

typedef struct {
    const char* cbName;
    const char* cbSignature;
    jniWrapper_t jniWrapper;
    jmethodID cbMethod;
//  params_t cbParams[];
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
			"callback2", "(I)V", JNI_WRAPPER_rVOID_pINT,
	},
	// callback3 [2]
	{
			"callback3", "(II)V", JNI_WRAPPER_rVOID_pINT_pINT,
	},
};

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
	//GetEnv tells whether the thread is already attached to the VM
	status = (*gJavaVM)->GetEnv(gJavaVM, (void **) &env, JNI_VERSION_1_4);
	LOGI("GetEnv status is: %d", status);
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
	if(cb == NULL)
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
	if(isAttached == 1)
		(*gJavaVM)->DetachCurrentThread(gJavaVM);
}

int callStaticMethodWrapper(int id, params_t *par) {
	LOGI("callStaticMethodWrapper called!");
	JNIEnv *env;
	int isAttached = 0;
	switch ((*gJavaVM)->GetEnv(gJavaVM, (void**)&env, JNI_VERSION_1_4))
	{
		case JNI_OK:
		break;
		case JNI_EDETACHED:
			if ((*gJavaVM)->AttachCurrentThread(gJavaVM, &env, NULL)!=0)
			{
			  LOGE("Could not attach current thread");
			}
			isAttached = 1;
		break;
		case JNI_EVERSION:
			LOGE("Invalid java version");
		break;
	}
	LOGI("Calling: %s with signature %s and params %d and %d.", cb[id].cbName, cb[id].cbSignature);
	switch (cb[id].jniWrapper) {
	  //
	  case JNI_WRAPPER_rVOID_pVOID:
		  (*env)->CallStaticVoidMethod(env, gClass, cb[id].cbMethod);
	  break;
	  case JNI_WRAPPER_rVOID_pINT:
	      (*env)->CallStaticVoidMethod(env, gClass, cb[id].cbMethod, par[0].p_int);
	  break;
	  case JNI_WRAPPER_rVOID_pINT_pINT:
		  (*env)->CallStaticVoidMethod(env, gClass, cb[id].cbMethod, par[0].p_int, par[1].p_int);
	  break;
	}
	if (isAttached == 1)
		(*gJavaVM)->DetachCurrentThread(gJavaVM);
	return 0;
}

void daemonStart() {
	LOGI("daemonStart called!");
	flag = 1;
	params_t params[1];
	params[0].p_int = 56;
//	params[1].p_int = 21;
	/*
	 * The callStaticMethodWrapper takes (methodID, params)
	 */
	callStaticMethodWrapper(1, params);
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
