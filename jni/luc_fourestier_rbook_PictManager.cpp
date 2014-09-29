/*
 * luc_fourestier_rbook_PictManager.cpp
 *
 *  Created on: Sep 26, 2014
 *      Author: befourel
 */

#include "luc_fourestier_rbook_PictManager.h"
#include "RBook/PictManager.h"
#include "RBook/Log.h"

#define TAG "JNIPictManager"

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
    case RBook::PictManager::ERROR_CANNOT_INFLATE:
        LOG_E(TAG, "java/io/IOException");
        JNIThrowException(env, "java/io/IOException", NULL);
        break;
    case RBook::PictManager::ERROR_PICT_NOT_FOUND:
        LOG_W(TAG, "java/lang/IndexOutOfBoundsException");
        JNIThrowException(env, "java/lang/IndexOutOfBoundsException", NULL);
        break;
    default:
        LOG_E(TAG, "java/lang/RuntimeException");
        JNIThrowException(env, "java/lang/RuntimeException", "unknown error");
        break;
    }
}

// PictManager instances through JNI

static jfieldID NativeInstanceFieldID;

static void InitializeInstanceField(JNIEnv* env, jobject thiz) {
    jclass clazz;

    // Connect instances through JNI
    clazz = env->FindClass("luc/fourestier/rbook/PictManager");
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

static RBook::PictManager* GetInstance(JNIEnv* env, jobject thiz) {
    return reinterpret_cast<RBook::PictManager*>(env->GetIntField(thiz, NativeInstanceFieldID));
}

static void SetInstance(JNIEnv* env, jobject thiz, const RBook::PictManager* inst) {
    env->SetIntField(thiz, NativeInstanceFieldID, (int) inst);
}

// JNI API

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_PictManager__1Populate(JNIEnv *env, jobject thiz, jstring jpath, jstring jrootdir) {
    InitializeInstanceField(env, thiz);

    // Create and initialize PictManager
    const char* charrootdir = env->GetStringUTFChars(jrootdir, NULL);
    std::string cpprootdir(charrootdir);
    env->ReleaseStringUTFChars(jrootdir, charrootdir);

    const char* charpath = env->GetStringUTFChars(jpath, NULL);
    std::string cpppath(charpath);
    env->ReleaseStringUTFChars(jpath, charpath);

    RBook::PictManager *pm = new RBook::PictManager();
    if (pm == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", NULL);
        return;
    }

    RBook::Error error = pm->Populate(cpppath, cpprootdir);
    JNIThrowOnError(env, error);

    // Register instance at Java level
    SetInstance(env, thiz, pm);
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_PictManager__1GetPictListSerialized(JNIEnv *env, jobject thiz) {
    RBook::PictManager *pm = GetInstance(env, thiz);
    if (pm == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0;
    }

    // Get book list.
    jstring result;
    std::list<std::string> list;
    RBook::Error error = pm->GetPictList(list);
    JNIThrowOnError(env, error);
    if (error != RBook::ERROR_OK) {
        return result;
    }

    // Serialize list and return it
    std::string serializedlist;
    for (std::list<std::string>::iterator it = list.begin(); it != list.end(); ++it) {
        serializedlist += *it + ";";
    }
    result = env->NewStringUTF(serializedlist.c_str());

    return result;
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_PictManager__1GetPict(JNIEnv *env, jobject thiz, jstring jpict, jint resolution) {
    RBook::PictManager *pm = GetInstance(env, thiz);
    if (pm == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0;
    }

    const char* charpict = env->GetStringUTFChars(jpict, NULL);
    std::string cpppict(charpict);
    env->ReleaseStringUTFChars(jpict, charpict);

    jstring result;
    std::string path;
    RBook::Error error = pm->GetPict(cpppict, resolution, path);
    JNIThrowOnError(env, error);
    if (error != RBook::ERROR_OK) {
        return result;
    }
    result = env->NewStringUTF(path.c_str());

    return result;
}

