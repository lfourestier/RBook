/*
 * luc_fourestier_rbook_RoadBook.cpp
 *
 *  Created on: Aug 26, 2014
 *      Author: Luc Fourestier
 */

#include "luc_fourestier_rbook_RoadBook.h"
#include "RBook/RoadBook.h"
#include "RBook/Log.h"

#define TAG "JNIRoadBook"

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
    case RBook::RoadBook::ERROR_MAL_FORMATTED_BOOK:
    case RBook::RoadBook::ERROR_NO_BOOK_FILE:
        LOG_E(TAG, "java/io/IOException");
        JNIThrowException(env, "java/io/IOException", NULL);
        break;
    case RBook::RoadBook::ERROR_REACHED_END:
    case RBook::RoadBook::ERROR_REACHED_START:
        LOG_I(TAG, "java/lang/IndexOutOfBoundsException");
        JNIThrowException(env, "java/lang/IndexOutOfBoundsException", NULL);
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
        clazz = env->FindClass("luc/fourestier/rbook/RoadBook");
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

static RBook::RoadBook* GetInstance(JNIEnv* env, jobject thiz) {
    return reinterpret_cast<RBook::RoadBook*>(env->GetIntField(thiz, NativeInstanceFieldID));
}

static void SetInstance(JNIEnv* env, jobject thiz, const RBook::RoadBook* inst) {
    env->SetIntField(thiz, NativeInstanceFieldID, (int) inst);
}

// JNI API

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetBookName(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *mb = GetInstance(env, thiz);
    if (mb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return env->NewStringUTF("");
    }

    return env->NewStringUTF(mb->Title.c_str());
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetBookDescription(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return env->NewStringUTF("");
    }

    return env->NewStringUTF(rb->Description.c_str());
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetBookLocation(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return env->NewStringUTF("");
    }

    return env->NewStringUTF(rb->Location.c_str());
}

JNIEXPORT jfloat JNICALL Java_luc_fourestier_rbook_RoadBook__1GetBookTotalDistance(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0.0;
    }

    return rb->TotalDistance;
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetCurrentPointName(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return env->NewStringUTF("");
    }

    RBook::RoadPoint *rp = NULL;
    RBook::Error error = rb->GetCurrentPoint(rp);
    JNIThrowOnError(env, error);
    if (rp == NULL) {
        LOG_I(TAG, "Road point null!");
        return env->NewStringUTF("");
    }

    LOG_V(TAG, "Current point name: %s", rp->Name.c_str());
    return env->NewStringUTF(rp->Name.c_str());
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetCurrentPointObservation(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return env->NewStringUTF("");
    }

    RBook::RoadPoint *rp = NULL;
    RBook::Error error = rb->GetCurrentPoint(rp);
    JNIThrowOnError(env, error);
    if (rp == NULL) {
        LOG_I(TAG, "Road point null!");
        return env->NewStringUTF("");
    }

    return env->NewStringUTF(rp->Observation.c_str());
}

JNIEXPORT jfloat JNICALL Java_luc_fourestier_rbook_RoadBook__1GetCurrentPointKilometer(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0.0;
    }

    RBook::RoadPoint *rp = NULL;
    RBook::Error error = rb->GetCurrentPoint(rp);
    JNIThrowOnError(env, error);
    if (rp == NULL) {
        LOG_I(TAG, "Road point null!");
        return 0.0;
    }

    return rp->Kilometer;
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetCurrentPointType(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return env->NewStringUTF("");
    }

    RBook::RoadPoint *rp = NULL;
    RBook::Error error = rb->GetCurrentPoint(rp);
    JNIThrowOnError(env, error);
    if (rp == NULL) {
        LOG_I(TAG, "Road point null!");
        return env->NewStringUTF("");
    }

    return env->NewStringUTF(rp->Type.c_str());
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetCurrentPointDirection(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return env->NewStringUTF("");
    }

    RBook::RoadPoint *rp = NULL;
    RBook::Error error = rb->GetCurrentPoint(rp);
    JNIThrowOnError(env, error);
    if (rp == NULL) {
        LOG_I(TAG, "Road point null!");
        return env->NewStringUTF("");
    }

    return env->NewStringUTF(rp->Direction.c_str());
}

JNIEXPORT jfloat JNICALL Java_luc_fourestier_rbook_RoadBook__1GetCurrentPointDistanceToNext(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0.0;
    }

    float distance = 0.0;
    RBook::Error error = rb->GetDistanceToNextPoint(distance);
    JNIThrowOnError(env, error);

    return distance;
}

JNIEXPORT jfloat JNICALL Java_luc_fourestier_rbook_RoadBook__1GetCurrentPointDistanceFromPrevious(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0.0;
    }

    float distance = 0.0;
    RBook::Error error = rb->GetDistanceFromPreviousPoint(distance);
    JNIThrowOnError(env, error);

    return distance;
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadBook__1Next(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    RBook::Error error = rb->Next();
    JNIThrowOnError(env, error);
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadBook__1Previous(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    RBook::Error error = rb->Previous();
    JNIThrowOnError(env, error);
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadBook__1Reset(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    RBook::Error error = rb->Reset();
    JNIThrowOnError(env, error);
}
