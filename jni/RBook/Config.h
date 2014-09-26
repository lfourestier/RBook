/*
 * Config.h
 *
 *  Created on: Sep 18, 2014
 *      Author: befourel
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// System

#define CONFIG_PATH_DELIMITER "/"

// Erros bases

#define CONFIG_GENERIC_ERROR_BASE 0
#define CONFIG_ROADBOOK_ERROR_BASE 1000
#define CONFIG_ROADPOINT_ERROR_BASE 2000
#define CONFIG_BOOKMANAGER_ERROR_BASE 3000
#define CONFIG_ARCHIVE_ERROR_BASE 4000
#define CONFIG_FILEUTILS_ERROR_BASE 5000
#define CONFIG_PICTMANAGER_ERROR_BASE 6000

// Logs

#define LOG_ENABLE //! Enable logs. Comment it to disable log macros below.

// Mutually exclusive options
//#define LOG_FILELINE //! Will add file name and line number to the log.
//#define LOG_FUNCLINE //! Will add function name and line number to the log.

// Runtime options
#define LOG_ANDROID //!< Enable android native log. @todo adapt automatically with platform
//#define LOG_FILE   //!< Enable file log.
//#define LOG_PRINTF //!< Enable printf log (Console log)

#define LOG_MIN_LEVEL Log::LOG_VERBOSE //! Define the minimum level to be logged


#endif /* CONFIG_H_ */
