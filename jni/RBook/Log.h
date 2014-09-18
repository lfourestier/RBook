/*
 * Log.h
 *
 *  Created on: Aug 28, 2014
 *      Author: Luc Fourestier
 */

/**
 * @note You should prefer the macros than calling directly the Log methods. Once disabled macros do not have anymore effects on your code.
 */

#ifndef LOG_H_
#define LOG_H_

#include "Config.h"

// Preprocessing options

//#define LOG_ENABLE //! Enable logs. Comment it to disable log macros below.
// Mutually exclusive options
//#define LOG_FILELINE //! Will add file name and line number to the log.
//#define LOG_FUNCLINE //! Will add function name and line number to the log.

// Runtime options

//#define LOG_ANDROID //!< Enable android native log. @todo adapt automatically with platform
//#define LOG_FILE   //!< Enable file log.
//#define LOG_PRINTF //!< Enable printf log (Console log)

//#define LOG_MIN_LEVEL Log::LOG_VERBOSE //! Define the minimum level to be logged

#ifdef LOG_ANDROID
#include <android/log.h>
#endif

// Log macros

#ifdef LOG_ENABLE
#ifdef LOG_FILELINE

#define LOG_V(tag, stringformat, ...) Log::Verbose(tag, __FILE__, __LINE__, stringformat , ##__VA_ARGS__)
#define LOG_D(tag, stringformat, ...) Log::Debug(tag, __FILE__, __LINE__, stringformat , ##__VA_ARGS__)
#define LOG_I(tag, stringformat, ...) Log::Info(tag, __FILE__, __LINE__, stringformat , ##__VA_ARGS__)
#define LOG_W(tag, stringformat, ...) Log::Warning(tag, __FILE__, __LINE__, stringformat , ##__VA_ARGS__)
#define LOG_E(tag, stringformat, ...) Log::Error(tag, __FILE__, __LINE__, stringformat , ##__VA_ARGS__)
#define LOG_F(tag, stringformat, ...) Log::Fatal(tag, __FILE__, __LINE__, stringformat , ##__VA_ARGS__)

#elif defined(LOG_FUNCLINE)

#define LOG_V(tag, stringformat, ...) Log::Verbose(tag, __func__, __LINE__, stringformat , ##__VA_ARGS__)
#define LOG_D(tag, stringformat, ...) Log::Debug(tag, __func__, __LINE__, stringformat , ##__VA_ARGS__)
#define LOG_I(tag, stringformat, ...) Log::Info(tag, __func__, __LINE__, stringformat , ##__VA_ARGS__)
#define LOG_W(tag, stringformat, ...) Log::Warning(tag, __func__, __LINE__, stringformat , ##__VA_ARGS__)
#define LOG_E(tag, stringformat, ...) Log::Error(tag, __func__, __LINE__, stringformat , ##__VA_ARGS__)
#define LOG_F(tag, stringformat, ...) Log::Fatal(tag, __func__, __LINE__, stringformat , ##__VA_ARGS__)

#else

#define LOG_V(tag, stringformat, ...) Log::Verbose(tag, stringformat , ##__VA_ARGS__)
#define LOG_D(tag, stringformat, ...) Log::Debug(tag, stringformat , ##__VA_ARGS__)
#define LOG_I(tag, stringformat, ...) Log::Info(tag, stringformat , ##__VA_ARGS__)
#define LOG_W(tag, stringformat, ...) Log::Warning(tag, stringformat , ##__VA_ARGS__)
#define LOG_E(tag, stringformat, ...) Log::Error(tag, stringformat , ##__VA_ARGS__)
#define LOG_F(tag, stringformat, ...) Log::Fatal(tag, stringformat , ##__VA_ARGS__)

#endif
#else

#define LOG_V(tag, stringformat, ...)
#define LOG_D(tag, stringformat, ...)
#define LOG_I(tag, stringformat, ...)
#define LOG_W(tag, stringformat, ...)
#define LOG_E(tag, stringformat, ...)
#define LOG_F(tag, stringformat, ...)

#endif

/**
 * Log messages.
 */
class Log {

public:

    enum LogLevels {
        LOG_ALL = 0,
        LOG_VERBOSE = 0,
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR,
        LOG_FATAL,
        LOG_NONE
    };

    /**
     * Set the minimum log level. All logs below that level will be ignored.
     *
     * @param level: The min level
     * @return The new current level (Should be identical to level param)
     */
    static LogLevels SetMinLevel(LogLevels level);

    static void Verbose(const char * const tag, const char * const msg, ...);
    static void Verbose(const char * const tag, const char* file, int line,
            const char * const msg, ...);

    static void Debug(const char * const tag, const char * const msg, ...);
    static void Debug(const char * const tag, const char* file, int line,
            const char * const msg, ...);

    static void Info(const char * const tag, const char * const msg, ...);
    static void Info(const char * const tag, const char* file, int line,
            const char * const msg, ...);

    static void Warning(const char * const tag, const char * const msg, ...);
    static void Warning(const char * const tag, const char* file, int line,
            const char * const msg, ...);

    static void Error(const char * const tag, const char * const msg, ...);
    static void Error(const char * const tag, const char* file, int line,
            const char * const msg, ...);

    static void Fatal(const char * const tag, const char * const msg, ...);
    static void Fatal(const char * const tag, const char* file, int line,
            const char * const msg, ...);

private:
    static LogLevels MinLevel;
};

#endif /* LOG_H_ */
