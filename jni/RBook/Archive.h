/*
 * Archive.h
 *
 *  Created on: Sep 18, 2014
 *      Author: befourel
 */

#ifndef ARCHIVE_H_
#define ARCHIVE_H_

#include <string>
#include <vector>

#include "zip.h"
#include "Error.h"

#define ARCHIVE_ERROR_BASE CONFIG_ARCHIVE_ERROR_BASE

namespace RBook {

/**
 * Helper class for archiving files.
 * Archive is a flat structure of files. If any directories or subdirectories are specified in the path of the file to be archived,
 * it will be lost/discarded/flatten.
 */
class Archive {
public:
    //! Archive Error list
    enum ERROR {
        ERROR_FILE_NOT_FOUND = ARCHIVE_ERROR_BASE, //!< Did not find the file specified.
        ERROR_CANNOT_ARCHIVE, //!< File issue while archiving.
        ERROR_CANNOT_INFLATE, //!< Archive issue while inflating.
    };

    /**
     * Constructor
     */
    Archive(std::string filepath);

    /**
     * Destructor
     */
    ~Archive();

    /**
     * Append files in the archive.
     * @param files: The file arrays. Each element of the array must be full path of the file to archive (Ex: "/sdcard/RBook/file.mrb").
     * Inside the archive the file names will be shorten (Ex: if file = "/sdcard/RBook/file.mrb", it becomes "file.mrb" in the archive. Path is lost!)
     * @return @see ERROR
     */
    Error Append(std::vector<std::string> files);

    /**
     * Overwrite the archive with the files.
     * @param files: List of files (Full path as for @see Append).
     * Inside the archive the file names will be shorten (Ex: if file = "/sdcard/RBook/file.mrb", it becomes "file.mrb" in the archive. Path is lost!)
     * @return @see ERROR
     */
    Error Overwrite(std::vector<std::string> files);

    /**
     * Inflate (Decompress or de-archive) the archive.
     * @param dir: the directory path where all the files contained by the archive will be copied. Directory MUST exits!
     * @return @see ERROR
     * @note If the directory is not empty and contains a file that is also in the archive, it will be overwritten.
     * Advice: Prefer to use empty directories.
     */
    Error Inflate(std::string directory);

    /**
     * Deflate (compress) the content of a directory. (Sub directories not included!).
     * All files contained in that directory will be archived.
     * @param dir: The directory path. Directory MUST exist and contain at least one file!
     * @return @see ERROR
     */
    Error Deflate(std::string directory);

private:
    //! Full path for the archive file (file name and extension included).
    std::string FilePath;

    /**
     * Default constructor
     */
    Archive();

    /**
     * Add new files inot the archive.
     * @param files: List of files.
     * @param zfile: the archive file pointer.
     * @return @see ERROR
     */
    Error Add(std::vector<std::string> &files, zipFile &zfile);

};

} /* namespace RBook */
#endif /* ARCHIVEFILE_H_ */
