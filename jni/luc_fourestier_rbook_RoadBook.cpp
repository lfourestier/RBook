/*
 * luc_fourestier_rbook_RoadBook.cpp
 *
 *  Created on: Aug 26, 2014
 *      Author: Luc Fourestier
 */

#include "luc_fourestier_rbook_RoadBook.h"
#include "RBook/RoadBook.h"
#include "RBook/PictManager.h"
#include "RBook/Log.h"
#include "RBook/FileUtils.h"

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
    case RBook::RoadBook::ERROR_BOOK_NOT_FOUND:
    case RBook::RoadBook::ERROR_CANNOT_SAVE:
    case RBook::RoadBook::ERROR_CANNOT_CREATE:
    case RBook::FileUtils::ERROR_DIR_NOT_FOUND:
    case RBook::FileUtils::ERROR_FILE_NOT_FOUND:
        LOG_E(TAG, "java/io/IOException");
        JNIThrowException(env, "java/io/IOException", NULL);
        break;
    case RBook::RoadBook::ERROR_REACHED_END:
    case RBook::RoadBook::ERROR_REACHED_START:
    case RBook::RoadBook::ERROR_EMPTY_LIST:
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

    jstring result;
    RBook::RoadBook *mb = GetInstance(env, thiz);
    if (mb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }

    return env->NewStringUTF(mb->Bookname.c_str());
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetFilePath(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    jstring result;
    RBook::RoadBook *mb = GetInstance(env, thiz);
    if (mb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }

    return env->NewStringUTF(mb->FilePath.c_str());
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetTitle(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    jstring result;
    RBook::RoadBook *mb = GetInstance(env, thiz);
    if (mb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }

    return env->NewStringUTF(mb->Title.c_str());
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadBook__1SetTitle (JNIEnv *env, jobject thiz, jstring title) {
    InitializeInstanceField(env, thiz);

    const char* chartitle = env->GetStringUTFChars(title, NULL);
    std::string cpptitle(chartitle);
    env->ReleaseStringUTFChars(title, chartitle);

    RBook::RoadBook *roadbook = GetInstance(env, thiz);
    if (roadbook == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    roadbook->Title = cpptitle;
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetDescription(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    jstring result;
    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }

    return env->NewStringUTF(rb->Description.c_str());
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadBook__1SetDescription (JNIEnv *env, jobject thiz, jstring description) {
    InitializeInstanceField(env, thiz);

    const char* chardescription = env->GetStringUTFChars(description, NULL);
    std::string cppdescription(chardescription);
    env->ReleaseStringUTFChars(description, chardescription);

    RBook::RoadBook *roadbook = GetInstance(env, thiz);
    if (roadbook == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    roadbook->Description = cppdescription;
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetLocation(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    jstring result;
    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }

    return env->NewStringUTF(rb->Location.c_str());
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadBook__1SetLocation (JNIEnv *env, jobject thiz, jstring location) {
    InitializeInstanceField(env, thiz);

    const char* charlocation = env->GetStringUTFChars(location, NULL);
    std::string cpplocation(charlocation);
    env->ReleaseStringUTFChars(location, charlocation);

    RBook::RoadBook *roadbook = GetInstance(env, thiz);
    if (roadbook == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    roadbook->Location = cpplocation;
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetImage(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    jstring result;
    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }

    std::string image;
    RBook::Error error = rb->GetImage(image);
    JNIThrowOnError(env, error);

    return env->NewStringUTF(image.c_str());
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1CreateImagePath(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    jstring result;
    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }

    std::string imagepath;
    RBook::Error error = rb->CreateImagePath(imagepath);
    JNIThrowOnError(env, error);

    return env->NewStringUTF(imagepath.c_str());
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadBook__1SetImage (JNIEnv *env, jobject thiz, jstring imagepath) {
    InitializeInstanceField(env, thiz);

    const char* charimagepath = env->GetStringUTFChars(imagepath, NULL);
    std::string cppimagepath(charimagepath);
    env->ReleaseStringUTFChars(imagepath, charimagepath);

    RBook::RoadBook *roadbook = GetInstance(env, thiz);
    if (roadbook == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    RBook::Error error = roadbook->SetImage(cppimagepath);
    JNIThrowOnError(env, error);
}

JNIEXPORT jfloat JNICALL Java_luc_fourestier_rbook_RoadBook__1GetTotalDistance(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0.0;
    }

    return rb->TotalDistance;
}

JNIEXPORT jint JNICALL Java_luc_fourestier_rbook_RoadBook__1GetPointCount (JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *roadbook = GetInstance(env, thiz);
    if (roadbook == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0;
    }

    unsigned int count = 0;
    roadbook->GetPointCount(count);

    return (int) count;
}

JNIEXPORT jint JNICALL Java_luc_fourestier_rbook_RoadBook__1GetCurrentPoint (JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *roadbook = GetInstance(env, thiz);
    if (roadbook == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0;
    }

    RBook::RoadPoint *roadpoint = NULL;
    RBook::Error error = roadbook->GetCurrentPoint(roadpoint);
    JNIThrowOnError(env, error);

    return (int) roadpoint;
}

JNIEXPORT jint JNICALL Java_luc_fourestier_rbook_RoadBook__1GetPreviousPoint (JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *roadbook = GetInstance(env, thiz);
    if (roadbook == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0;
    }

    RBook::RoadPoint *roadpoint = NULL;
    RBook::Error error = roadbook->GetPreviousPoint(roadpoint);
    JNIThrowOnError(env, error);

    return (int) roadpoint;
}

JNIEXPORT jint JNICALL Java_luc_fourestier_rbook_RoadBook__1GetNextPoint (JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *roadbook = GetInstance(env, thiz);
    if (roadbook == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0;
    }

    RBook::RoadPoint *roadpoint = NULL;
    RBook::Error error = roadbook->GetNextPoint(roadpoint);
    JNIThrowOnError(env, error);

    return (int) roadpoint;
}

JNIEXPORT jfloat JNICALL Java_luc_fourestier_rbook_RoadBook__1GetDistanceToNext(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0.0;
    }

    float distance = 0.0;
    RBook::Error error = rb->GetDistanceToNext(distance);
    JNIThrowOnError(env, error);

    return distance;
}

JNIEXPORT jfloat JNICALL Java_luc_fourestier_rbook_RoadBook__1GetDistanceFromPrevious(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0.0;
    }

    float distance = 0.0;
    RBook::Error error = rb->GetDistanceFromPrevious(distance);
    JNIThrowOnError(env, error);

    return distance;
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetCurrentPointSpeech  (JNIEnv *env, jobject thiz, jint instance) {
    InitializeInstanceField(env, thiz);

    jstring result;
    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }
    RBook::PictManager *pictmgr = reinterpret_cast<RBook::PictManager*>(instance);
    std::string speech;
    RBook::Error error = rb->GetCurrentPointSpeech(pictmgr, speech);
    JNIThrowOnError(env, error);
    if (error != RBook::ERROR_OK) {
        return result;
    }

    return env->NewStringUTF(speech.c_str());

}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_RoadBook__1GetNextPointSpeech  (JNIEnv *env, jobject thiz, jint instance) {
    InitializeInstanceField(env, thiz);

    jstring result;
    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }
    RBook::PictManager *pictmgr = reinterpret_cast<RBook::PictManager*>(instance);
    std::string speech;
    RBook::Error error = rb->GetNextPointSpeech(pictmgr, speech);
    JNIThrowOnError(env, error);
    if (error != RBook::ERROR_OK) {
        return result;
    }

    return env->NewStringUTF(speech.c_str());

}

JNIEXPORT jint JNICALL Java_luc_fourestier_rbook_RoadBook__1AddNewPointBefore (JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *roadbook = GetInstance(env, thiz);
    if (roadbook == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0;
    }

    RBook::RoadPoint *roadpoint = NULL;
    RBook::Error error = roadbook->AddNewPointBefore(roadpoint);
    JNIThrowOnError(env, error);

    return (int) roadpoint;
}

JNIEXPORT jint JNICALL Java_luc_fourestier_rbook_RoadBook__1AddNewPointAfter (JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *roadbook = GetInstance(env, thiz);
    if (roadbook == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0;
    }

    RBook::RoadPoint *roadpoint = NULL;
    RBook::Error error = roadbook->AddNewPointAfter(roadpoint);
    JNIThrowOnError(env, error);

    return (int) roadpoint;
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadBook__1DeleteCurrentPoint(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    RBook::Error error = rb->DeleteCurrentPoint();
    JNIThrowOnError(env, error);
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

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_RoadBook__1End(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::RoadBook *rb = GetInstance(env, thiz);
    if (rb == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    RBook::Error error = rb->End();
    JNIThrowOnError(env, error);
}
