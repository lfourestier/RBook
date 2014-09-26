/*
 * Archive.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: befourel
 */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#include "Log.h"
#include "unzip.h"
#include "FileUtils.h"

#include "Archive.h"

#define TAG "Archive"

#define ARCHIVE_BUFFER_SIZE 256

namespace RBook {

Archive::Archive(std::string filepath) : FilePath(filepath) {
}

Archive::~Archive() {
}

Error Archive::Append(std::vector<std::string> files) {
    Error ret;

    if (!files.empty()) {
        LOG_V(TAG, "Appending %s", FilePath.c_str());

        zipFile zfile = zipOpen(FilePath.c_str(), APPEND_STATUS_ADDINZIP);
        if (zfile == NULL) {
            LOG_E(TAG, "Could not open archive file!");
            return ERROR_CANNOT_ARCHIVE;
        }

        ret = Add(files, zfile);

        zipClose(zfile, NULL);
    }

    return ret;
}

Error Archive::Overwrite(std::vector<std::string> files) {
    Error ret;

    if (!files.empty()) {
        LOG_V(TAG, "Overwriting %s", FilePath.c_str());

        zipFile zfile = zipOpen(FilePath.c_str(), APPEND_STATUS_CREATE);
        if (zfile == NULL) {
            LOG_E(TAG, "Could not create archive file!");
            return ERROR_CANNOT_ARCHIVE;
        }

        ret = Add(files, zfile);

        zipClose(zfile, NULL);
    }

    return ret;
}

Error Archive::Inflate(std::string directory) {
    Error ret;

    LOG_V(TAG, "Inflating %s", FilePath.c_str());

    unzFile unzipfile = unzOpen(FilePath.c_str());
    if (unzipfile == NULL) {
        LOG_E(TAG, "Could not open archive file!");
        return ERROR_CANNOT_INFLATE;
    }

    int err = unzGoToFirstFile(unzipfile);
    if (err != ZIP_OK) {
        LOG_E(TAG, "Go to first file in zip failed!");
        return ERROR_CANNOT_INFLATE;
    }

    bool endofarchive = false;
    do {
        unz_file_info fileinfo;
        char buffer[ARCHIVE_BUFFER_SIZE];
        std::string filepath = directory + FILEUTILS_PATH_DELIMITER;

        err = unzOpenCurrentFile(unzipfile);
        if (err != ZIP_OK) {
            LOG_E(TAG, "Cannot open current file!");
            ret = ERROR_CANNOT_INFLATE;
           break;
        }

        err = unzGetCurrentFileInfo(unzipfile, &fileinfo, buffer, ARCHIVE_BUFFER_SIZE, NULL, 0, NULL, 0);
        if (err != ZIP_OK) {
            LOG_E(TAG, "Cannot get current file name/info!");
            ret = ERROR_CANNOT_INFLATE;
            break;
        }
        filepath.append(buffer);

        FILE *filetounzip = fopen(filepath.c_str(),"wb");
        if (filetounzip == NULL) {
            LOG_E(TAG, "Error while opening file %s", filepath.c_str());
            ret = ERROR_CANNOT_INFLATE;
            break;
        }

        int writesize = 0;
        do {
            writesize = unzReadCurrentFile(unzipfile, buffer, ARCHIVE_BUFFER_SIZE);
            if (writesize > 0) {
                int size = fwrite(buffer, 1, writesize, filetounzip);
                if (size <= 0) {
                    LOG_E(TAG, "Could not write inflated data!");
                    ret = ERROR_CANNOT_INFLATE;
                    break;
                }
            }
            if (unzeof(unzipfile)) {
                break;
            }
        } while (writesize > 0);

        fclose(filetounzip);
        unzCloseCurrentFile(unzipfile);

        err = unzGoToNextFile(unzipfile);
        if (err == UNZ_END_OF_LIST_OF_FILE) {
            endofarchive = true;
            break;
        }
        else if (err != ZIP_OK) {
            LOG_E(TAG, "Cannot go to next file in zip!");
            ret = ERROR_CANNOT_INFLATE;
            break;
        }

        if (ret != ERROR_OK) {
            break;
        }

        LOG_V(TAG, "Inflated %s from archive!", filepath.c_str());
    } while (!endofarchive);

    unzClose(unzipfile);

    return ret;
}

Error Archive::Deflate(std::string directory) {
    Error ret;
    std::vector<std::string> files;

    try {
        DIR* dir = opendir(directory.c_str());
        if (dir != NULL) {
            struct dirent* file;

            while ((file = readdir(dir)) != NULL) {
                if (file->d_type != DT_REG) {
                    continue;
                }

                std::string filename(directory + FILEUTILS_PATH_DELIMITER);
                filename.append(file->d_name);
                files.push_back(filename);
            }
            closedir(dir);
        }
        else {
            ret = ERROR_FAIL;
        }

        ret = Overwrite(files);
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }


    return ret;
}

Error Archive::Add(std::vector<std::string> &files, zipFile &zfile){
    Error ret;

    for (int i = 0; i < files.size(); ++i) {
        zip_fileinfo fileinfo = {0}; // TODO Enter proper file info here.

        std::string filenameinzip(files[i]);
        ret = FileUtils::GetFileName(filenameinzip);
        if (ret != ERROR_OK) {
           break;
        }

        int err = zipOpenNewFileInZip(zfile, filenameinzip.c_str(), &fileinfo,
                         NULL,0,NULL,0,NULL,
                         Z_DEFLATED,
                         Z_DEFAULT_COMPRESSION);
        if (err != ZIP_OK) {
            LOG_E(TAG, "Error while opening new file in zip!");
            ret = ERROR_CANNOT_ARCHIVE;
            break;
        }

        FILE *filetozip = fopen(files[i].c_str(),"rb");
        if (filetozip == NULL) {
            LOG_E(TAG, "Error while opening file %s", files[i].c_str());
            ret = ERROR_CANNOT_ARCHIVE;
            break;
        }

        int readsize = 0;
        char buffer[ARCHIVE_BUFFER_SIZE];
        do {
            readsize = fread(buffer, 1, ARCHIVE_BUFFER_SIZE, filetozip);
            if (readsize > 0) {
                err = zipWriteInFileInZip (zfile, buffer, readsize);
                if (err != ZIP_OK) {
                    LOG_E(TAG, "Error while writing file in zip!");
                    ret = ERROR_CANNOT_ARCHIVE;
                    break;
                }
            }
            if (feof(filetozip)){
                break;
            }
        } while (readsize > 0);

        fclose(filetozip);
        zipCloseFileInZip(zfile);

        LOG_V(TAG, "Added %s in archive!", files[i].c_str());
    }

    return ret;
}

} /* namespace RBook */
