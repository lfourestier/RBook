/*
 * Config.h
 *
 *  Created on: Sep 18, 2014
 *      Author: befourel
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// Erros bases

#define ERROR_GENERIC_BASE 0
#define ERROR_ROADBOOK_BASE 1000
#define ERROR_ROADPOINT_BASE 2000
#define ERROR_BOOKMANAGER_BASE 3000
#define ERROR_ARCHIVEFILE_BASE 4000

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
