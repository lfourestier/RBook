/*
 * Log.cpp
 *
 *  Created on: Aug 28, 2014
 *      Author: Luc Fourestier
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "Log.h"

static void Print(Log::LogLevels level, const char * const tag,
        const char* file, int line, const char *msg, va_list arglist);

Log::LogLevels Log::MinLevel = LOG_MIN_LEVEL;

Log::LogLevels Log::SetMinLevel(Log::LogLevels level) {
    return MinLevel = level;
}

void Log::Verbose(const char * const tag, const char * const msg, ...) {
    if (LOG_VERBOSE < MinLevel) {
        return;
    }

    if (NULL == tag) {
        return;
    }

    va_list arglist;
    va_start(arglist, msg);
    Print(LOG_VERBOSE, tag, NULL, 0, msg, arglist);
    va_end(arglist);
}

void Log::Verbose(const char * const tag, const char* file, int line,
        const char * const msg, ...) {
    if (LOG_VERBOSE < MinLevel) {
        return;
    }

    if (NULL == tag) {
        return;
    }

    va_list arglist;
    va_start(arglist, msg);
    Print(LOG_VERBOSE, tag, file, line, msg, arglist);
    va_end(arglist);
}

void Log::Debug(const char * const tag, const char * const msg, ...) {
    if (LOG_DEBUG < MinLevel) {
        return;
    }

    if (NULL == tag) {
        return;
    }

    va_list arglist;
    va_start(arglist, msg);
    Print(LOG_DEBUG, tag, NULL, 0, msg, arglist);
    va_end(arglist);
}

void Log::Debug(const char * const tag, const char* file, int line,
        const char * const msg, ...) {
    if (LOG_DEBUG < MinLevel) {
        return;
    }

    if (NULL == tag) {
        return;
    }

    va_list arglist;
    va_start(arglist, msg);
    Print(LOG_DEBUG, tag, file, line, msg, arglist);
    va_end(arglist);
}

void Log::Info(const char * const tag, const char * const msg, ...) {
    if (LOG_INFO < MinLevel) {
        return;
    }

    if (NULL == tag) {
        return;
    }

    va_list arglist;
    va_start(arglist, msg);
    Print(LOG_INFO, tag, NULL, 0, msg, arglist);
    va_end(arglist);
}

void Log::Info(const char * const tag, const char* file, int line,
        const char * const msg, ...) {
    if (LOG_INFO < MinLevel) {
        return;
    }

    if (NULL == tag) {
        return;
    }

    va_list arglist;
    va_start(arglist, msg);
    Print(LOG_INFO, tag, file, line, msg, arglist);
    va_end(arglist);
}

void Log::Warning(const char * const tag, const char * const msg, ...) {
    if (LOG_WARNING < MinLevel) {
        return;
    }

    if (NULL == tag) {
        return;
    }

    va_list arglist;
    va_start(arglist, msg);
    Print(LOG_WARNING, tag, NULL, 0, msg, arglist);
    va_end(arglist);
}

void Log::Warning(const char * const tag, const char* file, int line,
        const char * const msg, ...) {
    if (LOG_WARNING < MinLevel) {
        return;
    }

    if (NULL == tag) {
        return;
    }

    va_list arglist;
    va_start(arglist, msg);
    Print(LOG_WARNING, tag, file, line, msg, arglist);
    va_end(arglist);
}

void Log::Error(const char * const tag, const char * const msg, ...) {
    if (LOG_ERROR < MinLevel) {
        return;
    }

    if (NULL == tag) {
        return;
    }

    va_list arglist;
    va_start(arglist, msg);
    Print(LOG_ERROR, tag, NULL, 0, msg, arglist);
    va_end(arglist);
}

void Log::Error(const char * const tag, const char* file, int line,
        const char * const msg, ...) {
    if (LOG_ERROR < MinLevel) {
        return;
    }

    if (NULL == tag) {
        return;
    }

    va_list arglist;
    va_start(arglist, msg);
    Print(LOG_ERROR, tag, file, line, msg, arglist);
    va_end(arglist);
}

void Log::Fatal(const char * const tag, const char * const msg, ...) {
    if (LOG_FATAL < MinLevel) {
        return;
    }

    if (NULL == tag) {
        return;
    }

    va_list arglist;
    va_start(arglist, msg);
    Print(LOG_FATAL, tag, NULL, 0, msg, arglist);
    va_end(arglist);
}

void Log::Fatal(const char * const tag, const char* file, int line,
        const char * const msg, ...) {
    if (LOG_FATAL < MinLevel) {
        return;
    }

    if (NULL == tag) {
        return;
    }

    va_list arglist;
    va_start(arglist, msg);
    Print(LOG_FATAL, tag, file, line, msg, arglist);
    va_end(arglist);
}

// Static

#ifdef LOG_ANDROID
//! Translation table to android log levels
const static int AndroidLogLevel[] =
        { ANDROID_LOG_VERBOSE, ANDROID_LOG_DEBUG, ANDROID_LOG_INFO,
                ANDROID_LOG_WARN, ANDROID_LOG_ERROR, ANDROID_LOG_FATAL, };
#endif

#if (defined(LOG_FILE) || defined(LOG_PRINTF))
//! translation table to string log levels
const static char *StringLogLevel[] = {
    "Verbose",
    "Debug",
    "Info",
    "Warning",
    "Error",
    "Fatal"
};
#endif

#ifdef LOG_FILE
FILE * LogFile = NULL;
#endif

//! Max buffer size for messages
#define MAX_LOG_SIZE  1024

/**
 * Print the message into log destination depending on the options
 *
 * @param level: Log level
 * @param tag: Component tag
 * @param file: File name (Can be null)
 * @param line: line number (If file is null, this will be discarded)
 * @param msg: the format log message (printf like formatted string)
 * @param arglist: Argument list that will be taken by the formatted message string
 */
void Print(Log::LogLevels level, const char * const tag, const char* file,
        int line, const char *msg, va_list arglist) {
    char str[MAX_LOG_SIZE + 1];
    int size = MAX_LOG_SIZE;
    char* ptr = str;

    str[MAX_LOG_SIZE] = 0;

    // Insert File and Line
    if (file != NULL) {
        ::snprintf(ptr, MAX_LOG_SIZE, "%s:%d: ", file, line);
        size -= ::strlen(ptr);
        ptr += ::strlen(ptr);
    }

    // Insert message
    ::vsnprintf(ptr, size, msg, arglist);

    // Print log
#ifdef LOG_FILE
    if (LogFile == NULL) {
        LogFile = fopen("RBook.log", "a");
    }
    if (LogFile != NULL) {
        fprintf(LogFile, "%s: [%s] %s\n", StringLogLevel[level], tag, str);
    }
#endif

#ifdef LOG_PRINTF
    printf("%s: [%s] %s\n", StringLogLevel[level], tag, str);
#endif

#ifdef LOG_ANDROID
    __android_log_print(AndroidLogLevel[level], tag, "%s", str);
#endif

}
