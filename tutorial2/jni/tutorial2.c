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
#include <pthread.h>

#define  LOG_TAG    "tutorial2"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static JavaVM *gJavaVM;
static jobject gInterfaceObject, gDataObject;
static jclass gClass;


/*
 * The enum below is used to switch between the various callback functions.
 * Basically the callback will depend on the return type rTYPE and on the presence
 * or absence of parameters. In case of parameters presence, a proper jvalue array
 * will be used.
 */
typedef enum {
  JNI_WRAPPER_rVOID = 0,
  JNI_WRAPPER_rVOID_p = 1,
  JNI_WRAPPER_rINT = 2,
  JNI_WRAPPER_rINT_p = 3,
  JNI_WRAPPER_rFLOAT = 4,
  JNI_WRAPPER_rFLOAT_p = 5
} jniWrapper_t;

typedef enum {
	INT = 0,
	FLOAT = 1,
	STRING = 2,
} nType;

typedef struct {
	int i;
	float f;
	char* s;
	nType type;
} nvalue;

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
  // cb[0]
  {
    "callback1", "()V", JNI_WRAPPER_rVOID,
  },
  // cb[1]
  {
    "callback2", "(IFLjava/lang/String;)I", JNI_WRAPPER_rINT_p,
  },
  // cb[2]
  {
	"callback3", "(Ljava/lang/String;)V", JNI_WRAPPER_rVOID_p,
  },
  // cb[2]
  {
  	"callback4", "(F)F", JNI_WRAPPER_rFLOAT_p,
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

/*********************************
 * ACTIVITY VERSION JNI FUNCTION *
 *********************************/

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

	int i = sizeof cb / sizeof cb[0];
	while(i--) {
		LOGI("Method %d is %s with signature %s", i, cb[i].cbName, cb[i].cbSignature);
		cb[i].cbMethod = (*env)->GetStaticMethodID(env, clazz, cb[i].cbName, cb[i].cbSignature);
		if(!cb[i].cbMethod) {
			LOGE("callback_handler: failed to get method ID");
			goto failure;
		}
	}

failure:
	if(isAttached)
		(*gJavaVM)->DetachCurrentThread(gJavaVM);
}

/********************************
 * SERVICE VERSION JNI FUNCTION *
 ********************************/

void
Java_com_android_tutorial2_MyService_init( JNIEnv* env, jobject thiz )
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

	int i = sizeof cb / sizeof cb[0];
	while(i--) {
		LOGI("Method %d is %s with signature %s", i, cb[i].cbName, cb[i].cbSignature);
		cb[i].cbMethod = (*env)->GetStaticMethodID(env, clazz, cb[i].cbName, cb[i].cbSignature);
		if(!cb[i].cbMethod) {
			LOGE("callback_handler: failed to get method ID");
			goto failure;
		}
	}

failure:
	if(isAttached)
		(*gJavaVM)->DetachCurrentThread(gJavaVM);
}

/*
 * The callStaticMethodWrapper takes as input the int mid number which identifies the callback method
 * to be called, the nvalue npar[] array, which contains the input parameters, if any, and the parSize
 * which indicates the number of parameters
 */

int callStaticMethodWrapper(int mid, nvalue npar[], int parSize) {
	LOGI("callStaticMethodWrapper called for cb[%d] method %s,signature %s, switch case %d", mid, cb[mid].cbName, cb[mid].cbSignature, cb[mid].jniWrapper);
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
	//Case with parameters:
	//Create a jvalue array with the same size of the nvalue array:
	jvalue jpar[parSize];
	if (parSize > 0) {
		//Handle mapping nvalue -> jvalue
		int i;
		for (i=0; i<parSize; i++) {
			switch (npar[i].type) {
			case INT:
				jpar[i].i = npar[i].i;
			break;
			case FLOAT:
				jpar[i].f = npar[i].f;
			break;
			case STRING:
				jpar[i].l = (*env)->NewStringUTF(env, npar[i].s);
			break;
			}
		}
	}
	switch (cb[mid].jniWrapper) {
	  case JNI_WRAPPER_rVOID:
		  (*env)->CallStaticVoidMethod(env, gClass, cb[mid].cbMethod);
	  break;
	  case JNI_WRAPPER_rINT:
		  (*env)->CallStaticIntMethod(env, gClass, cb[mid].cbMethod);
	  break;
	  case JNI_WRAPPER_rFLOAT:
		  (*env)->CallStaticFloatMethod(env, gClass, cb[mid].cbMethod);
	  break;
	  case JNI_WRAPPER_rVOID_p:
		  (*env)->CallStaticVoidMethodA(env, gClass, cb[mid].cbMethod, jpar);
	  break;
	  case JNI_WRAPPER_rINT_p:
		  (*env)->CallStaticIntMethodA(env, gClass, cb[mid].cbMethod, jpar);
	  break;
	  case JNI_WRAPPER_rFLOAT_p:
		  (*env)->CallStaticFloatMethodA(env, gClass, cb[mid].cbMethod, jpar);
	  break;
	}
	if(isAttached)
		(*gJavaVM)->DetachCurrentThread(gJavaVM);
	return 0;
}

/*
 * The *randomCaller switches between four callbacks declared above, preparing the input
 * parameters needed by each call.
 */

void *randomCaller() {
	flag = 1;
	int i=0;
	nvalue v1, v2, v3, npar[3];
	while (flag == 1) {
		switch(i){
			case 0:
				i=1;
				callStaticMethodWrapper(0, NULL, 0);
			break;
			case 1:
				i=2;
				v1.type = INT;
				v1.i = 12;
				v2.type = FLOAT;
				v2.f = 2.3;
				v3.type = STRING;
				v3.s = "string";
				npar[0] = v1;
				npar[1] = v2;
				npar[2] = v3;
				callStaticMethodWrapper(1, npar, 3);
			break;
			case 2:
				i=3;
				v1.type = STRING;
				v1.s = "string";
				npar[0] = v1;
				callStaticMethodWrapper(2, npar, 1);
			break;
			case 3:
				i=0;
				v1.type = FLOAT;
				v1.f = 2.3;
				npar[0] = v1;
				callStaticMethodWrapper(3, npar, 1);
			break;
		}
		sleep(2);
	}
}

void daemonStart() {
	LOGI("daemonStart called!");
	pthread_t thread;
	char *message = "Thread 1 started!";
	int iret;
	iret = pthread_create( &thread, NULL, randomCaller, NULL);
}

/*
 * We could have used the jobject below, to access to the view and to public non-static methods,
 * but the aim of this tutorial is to sumulate a native thread which can asynchronously interact
 * with the Java View, without necessarily being linked to the calling object.
 */

void
Java_com_android_tutorial2_Tutorial2Activity_foo1( JNIEnv* env, jobject thiz)
{
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
