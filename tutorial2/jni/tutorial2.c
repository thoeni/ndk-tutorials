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
  JNI_WRAPPER_rVOID_pINTA = 3,
  JNI_WRAPPER_rVOID_pOBJ = 4,
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
    jintArray inta;
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
	// callback4 [3]
	{
			"callback4", "([I)V", JNI_WRAPPER_rVOID_pINTA,
	},
	// callbackObject [4]
	{
			"callbackObject", "(Lcom/android/tutorial2/Param;)V", JNI_WRAPPER_rVOID_pOBJ,
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
	if (!gClass) {
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

int callStaticMethodWrapper(int id, jint ipar[], int isize, jfloat fpar[], int fsize, char* spar[], int ssize) {
	LOGI("callStaticMethodWrapper called!");
	JNIEnv *env;
	int isAttached = 0;
	jfieldID fid;
	jclass cls;
	jintArray iarr;
	jfloatArray farr;
	jmethodID constructor;
	jobject obj;
	jobjectArray sarr;
	int i=0;
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
	switch (cb[id].jniWrapper) {
	  //
	  case JNI_WRAPPER_rVOID_pVOID:
		  (*env)->CallStaticVoidMethod(env, gClass, cb[id].cbMethod);
	  break;
	  case JNI_WRAPPER_rVOID_pINTA:
		  cb[id].inta = (*env)->NewIntArray(env, isize);
		  (*env)->SetIntArrayRegion(env, cb[id].inta, 0, isize, ipar);
		  (*env)->CallStaticVoidMethod(env, gClass, cb[id].cbMethod, cb[id].inta);
	  break;
	  case JNI_WRAPPER_rVOID_pOBJ:
		  //Find the "Param" Class
		  cls = (*env)->FindClass(env, "com/android/tutorial2/Param");
		  //###Create a new Array of integers###
		  iarr = (*env)->NewIntArray(env, isize);
		  //Fill the array with the integer input parameter
		  (*env)->SetIntArrayRegion(env, iarr, 0, isize, ipar);
		  //###Create a new Array of floats###
		  farr = (*env)->NewIntArray(env, fsize);
		  //Fill the array with the float input parameter
		  (*env)->SetFloatArrayRegion(env, farr, 0, fsize, fpar);
		  //###Create a new Array of Strings###
		  sarr = (*env)->NewObjectArray(env, ssize, (*env)->FindClass(env, "java/lang/String"), NULL);
		  for (i=0; i<ssize; i++)
			  (*env)->SetObjectArrayElement(env, sarr, i, (*env)->NewStringUTF(env, spar[i]));

		  //Find the constructor of the Param object, which takes as parameter an int array and a float array
		  constructor = (*env)->GetMethodID(env, cls, "<init>", "([I[F[Ljava/lang/String;)V");
		  //Create the Object with its constructor, and the arrays as parameters
		  obj = (*env)->NewObject(env, cls, constructor, iarr, farr, sarr);
		  //Call the callback method passing the object as input parameter
		  (*env)->CallStaticVoidMethod(env, gClass, cb[id].cbMethod, obj);
	  	  LOGI("OK");
	  break;
	}
	if (isAttached == 1)
		(*gJavaVM)->DetachCurrentThread(gJavaVM);
	return 0;
}

void daemonStart() {
	LOGI("daemonStart called!");
	flag = 1;
	jint iparams[2] = {0, 1};
	jfloat fparams[2] = {1.2, 3.2};
	char* sparams[2] = {"ab","cd"};
	//callStaticMethodWrapper(method-to-be-called, intArray, intArray-size, floatArray, floatArray-size, stringArray, stringArray-size)
	callStaticMethodWrapper(4, iparams, 2, fparams, 2, sparams, 2);
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
Java_com_android_tutorial2_Tutorial2Activity_foo2( JNIEnv* env, jobject thiz )
{
	daemonStop();
}
