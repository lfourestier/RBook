/*
 * FileUtils.h
 *
 *  Created on: Sep 19, 2014
 *      Author: befourel
 */

#ifndef FILEUTILS_H_
#define FILEUTILS_H_

#include <list>
#include <string>

#include "Error.h"
#include "Config.h"

#define FILEUTILS_PATH_DELIMITER CONFIG_PATH_DELIMITER
#define FILEUTILS_EXTENSION_DELIMITER "."

#define FILEUTILS_ERROR_BASE CONFIG_FILEUTILS_ERROR_BASE

namespace RBook {

class FileUtils {
public:
    //! FileUtils Error list
    enum ERROR {
        ERROR_FILE_NOT_FOUND = FILEUTILS_ERROR_BASE, //!< File not found.
        ERROR_DIR_NOT_FOUND, //!< Directory not found
    };

// File

    /**
     * Delete a file
     * @param file: Full path to the file
     * @return @see ERROR
     */
    static Error DeleteFile(std::string file);

    /**
     * Check if file exists.
     * @pre file string MUST NOT be empty.
     * @param file: File path.
     * @return @see ERROR
     */
    static Error FileExists(std::string file);

    /**
     * From a full path file name, returns a short file name (Ex: "/sdcard/RBook/file.mrb" => "file.mrb")
     * Delimiters are '\' and '/'.
     * @param filepath: Full path string that will be replaced by the short name.
     * @return @see ERROR
     */
    static Error GetFileName(std::string &filepath);

    /**
     * Extract the path from the full file path (ex: "/sdcard/RBook/file.mrb" => "/sdcard/RBook/")
     * @param filepath: Full path string that will be replaced by the path.
     * @return @see ERROR
     */
    static Error GetFilePath(std::string &filepath);

    /**
     * Extract the root name from the full file path (ex: "/sdcard/RBook/file.mrb" => "file")
     * @param filepath: Full path string that will be replaced by the root name.
     * @return @see ERROR
     */
    static Error GetFileRoot(std::string &filepath);

    /**
     * Remove the extension of the file (Ex: "/sdcard/RBook/file.mrb" => "/sdcard/RBook/file")
     * @param filepath: Full path
     * @return @see ERROR
     */
    static Error RemoveExtension(std::string &filepath);
// Directory

    /**
     * Check directory existence.
     *
     * @param directory: Directory path (Full path). Intermediate sub-directories in the path MUST exist.
     * @return @see ERROR
     */
    static Error DirectoryExists(std::string directory);

    /**
     * Create directory.
     *
     * @param directory: Directory path (Full path). Intermediate sub-directories in the path MUST exist.
     * @return @see ERROR
     */
    static Error MakeDirectory(std::string directory);

    /**
     * Remove the specified directory (And all its content).
     *
     * @param directory: Directory path (Full path).
     * @return @see ERROR
     */
    static Error RemoveDirectory(std::string directory);

    /**
     * List files of a certain extension contained by a directory.
     *
     * @param directory: Directory path (Full path). Directory MUST exist.
     * @param extension: Extension. Must contain the "." separator, eg, ".txt" (And not just "txt"!!!).
     * @param list: The returned list of file (Extension is stripped off from the filename. eg: "file.txt" becomes "file" in the list).
     * @return @see ERROR
     */
    static Error ListFilesRootInDir(std::string directory, std::string extension, std::list<std::string> &list);

    /**
     * List files of a certain extension contined by the directory
     * @param directory: Directory path (Full path). Directory MUST exist.
     * @param extension: Extension. Must contain the "." separator, eg, ".txt" (And not just "txt"!!!).
     * @param list: The returned list of file (Full path; Ex: "/sdcard/RBook/file.mrz").
     * @return
     */
    static Error ListFilesPathInDir(std::string directory, std::string extension, std::list<std::string> &list);
};

} /* namespace RBook */
#endif /* FILEUTILS_H_ */
