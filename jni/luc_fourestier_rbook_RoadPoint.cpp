/*
 * luc_fourestier_rbook_RoadPoint.cpp
 *
 *  Created on: Aug 26, 2014
 *      Author: Luc Fourestier
 */

#include "luc_fourestier_rbook_RoadPoint.h"
#include "RBook/RoadPoint.h"
#include "RBook/Log.h"

#define TAG "JNIRoadPoint"

// Exceptions and error translation

static void JNIThrowException(JNIEnv *env, const char* exception, const char *message) {
    jclass clazz = env->FindClass(exception);
    env->ThrowNew(clazz, message);
    env->DeleteLocalRef(clazz);
}

static void JNIThrowOnError(JNIEnv *env, RBook::Error error) {
    switch (ERROR_CODE(error)) {
    case RBook::ERROR_OK:
        break;
    default:
        LOG_E(TAG, "java/lang/RuntimeException");
        JNIThrowException(env, "java/lang/RuntimeException", "unknown error");
        break;
    }
}

// Instances through JNI

static jfieldID NativeInstanceFieldID;

static void InitializeInstanceField(JNIEnv* env, jobject thiz) {
    jclass clazz;

    if (NativeInstanceFieldID == NULL) {
        // Connect instances through JNI
        clazz = env->FindClass("luc/fourestier/rbook/RoadPoint");
        if (clazz == NULL) {
            JNIThrowException(env, "java/lang/RuntimeException", NULL);
            return;
        }

        NativeInstanceFieldID = env->GetFieldID(clazz, "nativeInstance", "I");
        env->DeleteLocalRef(clazz);
        if (NativeInstanceFieldID == NULL) {
            JNIThrowException(env, "java/lang/RuntimeException", NULL);
            return;
        }
    }
}

static RBook::RoadPoint* GetInstance(JNIEnv* env, jobject thiz) {
    return reinterpret_cast<RBook::RoadPoint*>(env->GetIntField(thiz, NativeInstanceFieldID));
}

static void SetInstance(JNIEnv* env, jobject thiz, const RBook::RoadPoint* inst) {
    env->SetIntField(thiz, NativeInstanceFieldID, (int) inst);
}

// JNI API

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadPoint__1GetDescription(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    jstring result;
    RBook::RoadPoint *roadpoint = GetInstance(env, thiz);
    if (roadpoint == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }

    return env->NewStringUTF(roadpoint->Description.c_str());
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadPoint__1SetDescription (JNIEnv *env, jobject thiz, jstring description) {
    InitializeInstanceField(env, thiz);

    const char* chardescription = env->GetStringUTFChars(description, NULL);
    std::string cppdescription(chardescription);
    env->ReleaseStringUTFChars(description, chardescription);

    RBook::RoadPoint *roadpoint = GetInstance(env, thiz);
    if (roadpoint == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    roadpoint->Description = cppdescription;
}

JNIEXPORT jfloat JNICALL Java_luc_fourestier_rbook_RoadPoint__1GetKilometer(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadPoint *roadpoint = GetInstance(env, thiz);
    if (roadpoint == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0.0;
    }

    return roadpoint->Kilometer;
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadPoint__1SetKilometer (JNIEnv *env, jobject thiz, jfloat kilometer) {
    InitializeInstanceField(env, thiz);

    RBook::RoadPoint *roadpoint = GetInstance(env, thiz);
    if (roadpoint == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    roadpoint->Kilometer = kilometer;
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadPoint__1GetType(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    jstring result;
    RBook::RoadPoint *roadpoint = GetInstance(env, thiz);
    if (roadpoint == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }

    return env->NewStringUTF(roadpoint->Type.c_str());
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadPoint__1SetType (JNIEnv *env, jobject thiz, jstring type) {
    InitializeInstanceField(env, thiz);

    const char* chartype = env->GetStringUTFChars(type, NULL);
    std::string cpptype(chartype);
    env->ReleaseStringUTFChars(type, chartype);

    RBook::RoadPoint *roadpoint = GetInstance(env, thiz);
    if (roadpoint == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    roadpoint->Type = cpptype;
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadPoint__1GetDirection(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    jstring result;
    RBook::RoadPoint *roadpoint = GetInstance(env, thiz);
    if (roadpoint == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }

    return env->NewStringUTF(roadpoint->Direction.c_str());
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadPoint__1SetDirection (JNIEnv *env, jobject thiz, jstring direction) {
    InitializeInstanceField(env, thiz);

    const char* chardirection = env->GetStringUTFChars(direction, NULL);
    std::string cppdirection(chardirection);
    env->ReleaseStringUTFChars(direction, chardirection);

    RBook::RoadPoint *roadpoint = GetInstance(env, thiz);
    if (roadpoint == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    roadpoint->Direction = cppdirection;
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadPoint__1GetImage(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    jstring result;
    RBook::RoadPoint *roadpoint = GetInstance(env, thiz);
    if (roadpoint == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }

    std::string image;
    RBook::Error error = roadpoint->GetImage(image);
    JNIThrowOnError(env, error);

    return env->NewStringUTF(image.c_str());
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadPoint__1SetImage (JNIEnv *env, jobject thiz, jstring imagepath) {
    InitializeInstanceField(env, thiz);

    const char* charimagepath = env->GetStringUTFChars(imagepath, NULL);
    std::string cppimagepath(charimagepath);
    env->ReleaseStringUTFChars(imagepath, charimagepath);

    RBook::RoadPoint *roadpoint = GetInstance(env, thiz);
    if (roadpoint == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    RBook::Error error = roadpoint->SetImage(cppimagepath);
    JNIThrowOnError(env, error);
}

JNIEXPORT jint JNICALL Java_luc_fourestier_rbook_RoadPoint__1GetNumber(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadPoint *roadpoint = GetInstance(env, thiz);
    if (roadpoint == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0.0;
    }

    return roadpoint->Number;
}

