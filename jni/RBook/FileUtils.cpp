/*
 * FileUtils.cpp
 *
 *  Created on: Sep 19, 2014
 *      Author: befourel
 */

#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fstream>

#include "Log.h"

#include "FileUtils.h"


#define TAG "FileUtils"

namespace RBook {

Error FileUtils::CopyFile(std::string source, std::string destination) {
    Error ret;

    try {
        std::ifstream inputfile(source.c_str(), std::ifstream::in|std::ifstream::binary);
        std::ofstream outputfile(destination.c_str(), std::ofstream::out|std::ofstream::trunc|std::ofstream::binary);

        if (inputfile.is_open() && outputfile.is_open()) {
            outputfile << inputfile.rdbuf();
            inputfile.close();
            outputfile.close();
        }
        else {
            ret = ERROR_FAIL;
        }
    } catch (std::exception &e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error FileUtils::DeleteFile(std::string file) {
    Error ret;

    if ((!file.empty()) && (FileUtils::FileExists(file) == ERROR_OK)) {
        if (remove(file.c_str()) != 0) {
            ret = ERROR_FAIL;
        }
    }
    else {
        ret = ERROR_FILE_NOT_FOUND;
    }

    return ret;
}

Error FileUtils::FileExists(std::string file) {
    Error ret;
    struct stat status;

    try {
        if (stat(file.c_str(), &status) == 0) {
            if (!S_ISREG(status.st_mode)) {
                ret = ERROR_FILE_NOT_FOUND;
            }
        }
        else {
            ret = ERROR_FILE_NOT_FOUND;
        }
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error FileUtils::GetFileName(std::string &filepath) {
    Error ret;
    try {
        unsigned int position = filepath.find_last_of(FILEUTILS_PATH_DELIMITER);
        filepath = filepath.substr(position+1);
    } catch (std::exception& e) {
        LOG_E(TAG, "Could extract file name from path!");
        ret = ERROR_FAIL;
    }
    return ret;
}

Error FileUtils::GetFilePath(std::string &filepath) {
    Error ret;
    try {
        unsigned int position = filepath.find_last_of(FILEUTILS_PATH_DELIMITER);
        filepath = filepath.substr(0, position);
    } catch (std::exception& e) {
        LOG_E(TAG, "Could extract path from file path!");
        ret = ERROR_FAIL;
    }
    return ret;
}

Error FileUtils::GetFileRoot(std::string &filepath) {
    Error ret;
    try {
        ret = GetFileName(filepath);
        if (ret != ERROR_OK) {
            return ret;
        }
        unsigned int position = filepath.find(FILEUTILS_EXTENSION_DELIMITER);
        filepath = filepath.substr(0, position);
    } catch (std::exception& e) {
        LOG_E(TAG, "Could extract root name from file path!");
        ret = ERROR_FAIL;
    }
    return ret;
}

Error FileUtils::RemoveExtension(std::string &filepath) {
    Error ret;
    try {
        unsigned int position = filepath.find(FILEUTILS_EXTENSION_DELIMITER);
        filepath = filepath.substr(0, position);
    } catch (std::exception& e) {
        LOG_E(TAG, "Could remove extension!");
        ret = ERROR_FAIL;
    }
    return ret;
}

Error FileUtils::DirectoryExists(std::string directory) {
    Error ret;
    struct stat status;

    try {
        if (stat(directory.c_str(), &status) == 0) {
            if (!S_ISDIR(status.st_mode)) {
                ret = ERROR_DIR_NOT_FOUND;
            }
        }
        else {
            ret = ERROR_DIR_NOT_FOUND;
        }
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error FileUtils::MakeDirectory(std::string directory) {
    Error ret;

    try {
        if (mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
            LOG_E(TAG, "Error while creating directory %s!", directory.c_str());
            ret = ERROR_FAIL;
        }
    }
    catch (std::exception& e) {
        LOG_E(TAG, "Exception while creating directory %s!", directory.c_str());
        ret = ERROR_FAIL;
    }

    return ret;
}

Error FileUtils::RemoveDirectory(std::string directory) {
    Error ret;

    try {
        DIR* dir = opendir(directory.c_str());
        if (dir != NULL) {
            struct dirent* file;

            while ((file = readdir(dir)) != NULL) {
                if (((strlen(file->d_name) == 1) && (file->d_name[0] = '.')) || ((strlen(file->d_name) == 2) && (file->d_name[0] = '.') && (file->d_name[1] = '.'))) {
                    // Avoid standard directories
                    continue;
                }

                std::string filepath(directory + FILEUTILS_PATH_DELIMITER);
                filepath.append(file->d_name);
                struct stat status;

                if (stat(filepath.c_str(), &status) == 0) {
                    if (S_ISDIR(status.st_mode)) {
                        ret = RemoveDirectory(filepath);
                        if (ret != ERROR_OK) {
                            LOG_W(TAG, "Error while removing directory %s", filepath.c_str());
                        }
                    }
                    else if (S_ISREG(status.st_mode)) {
                        int err = remove(filepath.c_str());
                        if (err != 0) {
                            LOG_W(TAG, "Error %i while removing file %s", errno, filepath.c_str());
                        }
                    }
                    else {
                        LOG_W(TAG, "Cannot remove %s: Not a file or dir!", filepath.c_str());
                    }
                }
                else {
                    // try anyway!
                    int err = remove(filepath.c_str());
                    if (err != 0) {
                        LOG_W(TAG, "Error %i while removing file %s", errno, filepath.c_str());
                    }
                }

            }
            closedir(dir);
        }
        else {
            LOG_W(TAG, "Directory struct is null!");
            ret = ERROR_FAIL;
        }

        int err = rmdir(directory.c_str());
        if (err != 0) {
            LOG_W(TAG, "Error %i while removing directory %s", errno, directory.c_str());
            ret = ERROR_FAIL;
        }
    }
    catch (std::exception& e) {
        LOG_E(TAG, "Error while removing directory: %s", e.what());
        ret = ERROR_FAIL;
    }

    return ret;
}

Error FileUtils::ListFilesRootInDir(std::string directory, std::string suffix, std::list<std::string> &list) {
    Error ret;

    try {
        if (!list.empty()) {
            list.clear();
        }

        DIR* dir = opendir(directory.c_str());
        if (dir != NULL) {
            struct dirent* file;

            while ((file = readdir(dir)) != NULL) {
                std::string filename(file->d_name);

                std::size_t pos = filename.find(suffix);
                if (pos == std::string::npos) {
                    continue;
                }

                filename.replace(pos, suffix.size(), "");
                list.push_back(filename);
            }
            closedir(dir);
        }
        else {
            ret = ERROR_FAIL;
        }
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error FileUtils::ListFilesPathInDir(std::string directory, std::string extension, std::list<std::string> &list) {
    Error ret;

    try {
        if (!list.empty()) {
            list.clear();
        }

        DIR* dir = opendir(directory.c_str());
        if (dir != NULL) {
            struct dirent* file;

            while ((file = readdir(dir)) != NULL) {
                std::string filename(file->d_name);

                std::size_t pos = filename.find(extension);
                if (pos == std::string::npos) {
                    continue;
                }

                filename = directory + "/" + filename;
                list.push_back(filename);
            }
            closedir(dir);
        }
        else {
            ret = ERROR_FAIL;
        }
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}
} /* namespace RBook */
