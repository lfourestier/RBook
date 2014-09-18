/*
 * ArchiveFile.h
 *
 *  Created on: Sep 18, 2014
 *      Author: befourel
 */

#ifndef ARCHIVEFILE_H_
#define ARCHIVEFILE_H_

#include <string>

#include "Error.h"

#define ARCHIVEFILE_ERROR_BASE ERROR_ARCHIVEFILE_BASE

namespace RBook {

/**
 * Helper class for archiving files.
 * Archive is a flat structure of files. If any directories or subdirectories are specified in the path of the file to be archived,
 * it will be lost/discarded/flatten.
 */
class ArchiveFile {
public:
    //! ArchiveFile Error list
    enum ERROR {
        ERROR_FILE_NOT_FOUND = ARCHIVEFILE_ERROR_BASE, //!< Did not find the file specified.
        ERROR_CANNOT_ARCHIVE, //!< File issue while archiving.
    };

    /**
     * Create a new archive file. Overwrite if already exits. Caller MUST check existence first.
     * @param filepath: Full file path where to create the archive, file name and extension include (Ex: "/sdcard/RBook/file.mrz").
     * @param file: The returned archive file object pointer.
     * @return @see ERROR
     */
    static Error Create(std::string filepath, ArchiveFile *&file);

    /**
     * Open the archive. The file MUST exist.
     * @param filepath: Full file path of the archive to open, file name and extension include (Ex: "/sdcard/RBook/file.mrz").
     * @param file: the returned file object.
     * @return @see ERROR
     */
    static Error Open(std::string filepath, ArchiveFile *&file);

    /**
     * Add files in the archive.
     * Inside the archive the file names will be shorten (Ex: if file = "/sdcard/RBook/file.mrb", it becomes "file.mrb" in the archive. Path is lost!)
     * @param files: The file arrays. Each element of the array must be full path of the file to archive (Ex: "/sdcard/RBook/file.mrb")
     * @return @see ERROR
     */
    Error Add(std::string files[]);

    /**
     * Inflate (Decompress or de-archive) the archive.
     * @param dir: the directory where all the files contained by the archive will be copied.
     * @return @see ERROR
     */
    Error Inflate(std::string dir);

    /**
     * Close the archive.
     * @param file: The archive object.
     * @return @see ERROR
     */
    static Error Close(ArchiveFile *&file);

private:
    /**
     * Default constructor
     */
    ArchiveFile();

    /**
     * Destructor
     */
    ~ArchiveFile();


};

} /* namespace RBook */
#endif /* ARCHIVEFILE_H_ */
