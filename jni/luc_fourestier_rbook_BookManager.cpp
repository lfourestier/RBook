/*
 * luc_fourestier_rbook_BookManager.cpp
 *
 *  Created on: Sep 2, 2014
 *      Author: befourel
 */

#include "luc_fourestier_rbook_BookManager.h"
#include "RBook/BookManager.h"
#include "RBook/Log.h"
#include "FileUtils.h"

#define TAG "JNIBookManager"

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
    case RBook::RoadBook::ERROR_CANNOT_SAVE:
    case RBook::RoadBook::ERROR_BOOK_NOT_FOUND:
    case RBook::FileUtils::ERROR_DIR_NOT_FOUND:
    case RBook::FileUtils::ERROR_FILE_NOT_FOUND:
        LOG_E(TAG, "java/io/IOException");
        JNIThrowException(env, "java/io/IOException", NULL);
        break;
    case RBook::BookManager::ERROR_ROADBOOK_EXISTS:
        LOG_E(TAG, "java/lang/IllegalArgumentException");
        JNIThrowException(env, "java/lang/IllegalArgumentException", NULL);
        break;
    case RBook::RoadBook::ERROR_EMPTY_LIST:
    case RBook::BookManager::ERROR_EMPTY_LIST:
    case RBook::BookManager::ERROR_BOOKNAME_NOT_FOUND:
        LOG_E(TAG, "java/lang/IndexOutOfBoundsException");
        JNIThrowException(env, "java/lang/IndexOutOfBoundsException", NULL);
        break;
    default:
        LOG_E(TAG, "java/lang/RuntimeException");
        JNIThrowException(env, "java/lang/RuntimeException", "unknown error");
        break;
    }
}

// BookManager instances through JNI

static jfieldID NativeInstanceFieldID;

static void InitializeInstanceField(JNIEnv* env, jobject thiz) {
    jclass clazz;

    // Connect instances through JNI
    clazz = env->FindClass("luc/fourestier/rbook/BookManager");
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

static RBook::BookManager* GetInstance(JNIEnv* env, jobject thiz) {
    return reinterpret_cast<RBook::BookManager*>(env->GetIntField(thiz, NativeInstanceFieldID));
}

static void SetInstance(JNIEnv* env, jobject thiz, const RBook::BookManager* inst) {
    env->SetIntField(thiz, NativeInstanceFieldID, (int) inst);
}

// JNI API

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_BookManager__1Create(JNIEnv *env, jobject thiz) {
    InitializeInstanceField(env, thiz);

    RBook::BookManager *bm = new RBook::BookManager();
    if (bm == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", NULL);
        return;
    }

    // Register instance at Java level
    SetInstance(env, thiz, bm);
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_BookManager__1Initialize(JNIEnv *env, jobject thiz, jstring jrootdir) {
    RBook::BookManager *bm = GetInstance(env, thiz);
    if (bm == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    // Initialize BookManager
    const char* charrootdir = env->GetStringUTFChars(jrootdir, NULL);
    std::string cpprootdir(charrootdir);
    env->ReleaseStringUTFChars(jrootdir, charrootdir);

    RBook::Error error = bm->Initialize(cpprootdir);
    JNIThrowOnError(env, error);

}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_BookManager__1GetBookDir(JNIEnv *env, jobject thiz) {
    jstring result;
    RBook::BookManager *bm = GetInstance(env, thiz);
    if (bm == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }

    // Get book directory.
    std::string bookdir;
    RBook::Error error = bm->GetBookDir(bookdir);
    JNIThrowOnError(env, error);
    if (error != RBook::ERROR_OK) {
        return result;
    }

    return env->NewStringUTF(bookdir.c_str());
}

JNIEXPORT jstring JNICALL Java_luc_fourestier_rbook_BookManager__1GetRoadBookListSerialized(JNIEnv *env, jobject thiz) {
    jstring result;
    RBook::BookManager *br = GetInstance(env, thiz);
    if (br == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return result;
    }

    // Get book list.
    std::list<std::string> booklist;
    RBook::Error error = br->GetRoadBookList(booklist);
    JNIThrowOnError(env, error);
    if (error != RBook::ERROR_OK) {
        return result;
    }

    // Serialize list and return it
    std::string serializedlist;
    for (std::list<std::string>::iterator it = booklist.begin(); it != booklist.end(); ++it) {
        serializedlist += *it + ";";
    }
//    LOG_V(TAG, "Serialized list of books: %s", serializedlist.c_str());

    return env->NewStringUTF(serializedlist.c_str());
}

JNIEXPORT jint JNICALL Java_luc_fourestier_rbook_BookManager__1GetRoadBook(JNIEnv *env, jobject thiz, jstring bookname) {
    RBook::BookManager *bm = GetInstance(env, thiz);
    if (bm == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0;
    }

    const char* charbookname = env->GetStringUTFChars(bookname, NULL);
    std::string cppbookname(charbookname);
    env->ReleaseStringUTFChars(bookname, charbookname);

    RBook::RoadBook *roadbook = NULL;
    RBook::Error error = bm->GetRoadBook(cppbookname, roadbook);
    JNIThrowOnError(env, error);

    return (int) roadbook;
}

JNIEXPORT jint JNICALL Java_luc_fourestier_rbook_BookManager__1CreateRoadBook(JNIEnv *env, jobject thiz, jstring bookname) {
    RBook::BookManager *bm = GetInstance(env, thiz);
    if (bm == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0;
    }

    const char* charbookname = env->GetStringUTFChars(bookname, NULL);
    std::string cppbookname(charbookname);
    env->ReleaseStringUTFChars(bookname, charbookname);

    RBook::RoadBook *roadbook = NULL;
    RBook::Error error = bm->CreateRoadBook(cppbookname, roadbook);
    JNIThrowOnError(env, error);

    return (int) roadbook;
}

JNIEXPORT jint JNICALL Java_luc_fourestier_rbook_BookManager__1ImportRoadBook (JNIEnv *env, jobject thiz, jstring filepath, jstring bookname, jboolean overwrite) {
    RBook::BookManager *bm = GetInstance(env, thiz);
    if (bm == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return 0;
    }

    const char* charbookname = env->GetStringUTFChars(bookname, NULL);
    std::string cppbookname(charbookname);
    env->ReleaseStringUTFChars(bookname, charbookname);

    const char* charfilepath = env->GetStringUTFChars(filepath, NULL);
    std::string cppfilepath(charfilepath);
    env->ReleaseStringUTFChars(filepath, charfilepath);

    RBook::RoadBook *roadbook = NULL;
    RBook::Error error = bm->ImportRoadBook(cppfilepath, cppbookname, (bool) overwrite, roadbook);
    JNIThrowOnError(env, error);

    return (int) roadbook;
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_BookManager__1SaveRoadBook(JNIEnv *env, jobject thiz, jint instance) {
    RBook::BookManager *bm = GetInstance(env, thiz);
    if (bm == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    RBook::RoadBook *roadbook = reinterpret_cast<RBook::RoadBook*>(instance);
    if (roadbook == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }
    bm->SaveRoadBook(roadbook);
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_BookManager__1ReleaseRoadBook(JNIEnv *env, jobject thiz, jint instance) {
    RBook::BookManager *bm = GetInstance(env, thiz);
    if (bm == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

    RBook::RoadBook *roadbook = reinterpret_cast<RBook::RoadBook*>(instance);
    if (roadbook == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }
    bm->ReleaseRoadBook(roadbook);
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_BookManager__1DeleteRoadBook (JNIEnv *env, jobject thiz, jstring bookname) {
    RBook::BookManager *bm = GetInstance(env, thiz);
    if (bm == NULL) {
        JNIThrowException(env, "java/lang/RuntimeException", "Instance is null");
        return;
    }

     const char* charbookname = env->GetStringUTFChars(bookname, NULL);
     std::string cppbookname(charbookname);
     env->ReleaseStringUTFChars(bookname, charbookname);

     RBook::Error error = bm->DeleteRoadBook(cppbookname);
     JNIThrowOnError(env, error);
}

JNIEXPORT void JNICALL Java_luc_fourestier_rbook_BookManager__1Release(JNIEnv *env, jobject thiz) {
    RBook::BookManager *bm = GetInstance(env, thiz);
    if (bm != NULL) {
        delete bm;
    }

    SetInstance(env, thiz, NULL);
}

