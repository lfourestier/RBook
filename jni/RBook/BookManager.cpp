/*
 * BookManager.cpp
 *
 *  Created on: Sep 1, 2014
 *      Author: Luc Fourestier
 */

#include <sys/stat.h>
#include <dirent.h>

#include "Log.h"

#include "BookManager.h"

#define BOOK_DIR "/RBook"
#define BOOK_EXTENSION ".mrb"

#define TAG "BookManager"

namespace RBook {

Error BookManager::Initialize(std::string rootdir) {
    Error ret;

    RBookDirectory = rootdir + BOOK_DIR;

    ret = DirectoryExists(RBookDirectory);
    if (ret == ERROR_DIR_NOT_FOUND) {
        LOG_D(TAG, "RBook dir does not exist.");
        ret = MakeDirectory(RBookDirectory); // TODO MakeDirectory does not work.
    }
    else if (ret == ERROR_OK) {
        LOG_D(TAG, "Check RBook dir for files.");
        ret = ListFilesInDir(RBookDirectory, BOOK_EXTENSION, ListOfBooks);
    }

    return ret;
}

Error BookManager::FetchRoadBooks(std::list<std::string> fetchdirs) {
    Error ret;

    if (!RBookDirectory.empty()) {
        // TODO Implement FetchRoadBooks
    }
    else {
        ret = ERROR_NOT_INITIALIZED;
    }

    return ret;
}

Error BookManager::GetRoadBookList(std::list<std::string> &booklist)
{
    Error ret;

    try {
        if (!ListOfBooks.empty()) {
            // Copy list
             for (std::list<std::string>::iterator i=ListOfBooks.begin(); i != ListOfBooks.end(); ++i) {
                 booklist.push_front(*i);
             }
         }
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error BookManager::GetRoadBook(std::string bookname, RoadBook *& roadbook) {
    Error ret;

    try {
        if (!ListOfBooks.empty()) {
            ret = ERROR_BOOKNAME_NOT_FOUND;

            for (std::list<std::string>::iterator i=ListOfBooks.begin(); i != ListOfBooks.end(); ++i) {
                if (bookname.compare(*i) == 0) {
                    roadbook = new RoadBook();
                    roadbook->FilePath = RBookDirectory + "/" + bookname + BOOK_EXTENSION;
                    ret = roadbook->Load();
                    LOG_D("LFOR", "ret = %i", ret.Code); // TODO For debug only. Remove.
                    break;
                }
            }
        }
        else {
            ret = ERROR_BOOKNAME_NOT_FOUND;
        }
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error BookManager::ReleaseRoadBook(RoadBook *& roadbook) {
    Error ret;

    try {
        if (roadbook != NULL) {
            delete roadbook;
            roadbook = NULL;
        }
    } catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

BookManager::~BookManager() {
    try {
        // Delete book list
        while (!ListOfBooks.empty()) {
            ListOfBooks.pop_back();
        }
    }
    catch (std::exception& e) {
        // Do nothing
    }
}

// private

Error BookManager::DirectoryExists(std::string directory) {
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

Error BookManager::MakeDirectory(std::string directory) {
    Error ret;

    try {
        if (mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
            ret = ERROR_FAIL;
        }
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error BookManager::ListFilesInDir(std::string directory, std::string extension, std::list<std::string> &list) {
    Error ret;

    try {
        DIR* dir = opendir(directory.c_str());
        if (dir != NULL) {
            struct dirent* file;

            LOG_V(TAG, "Books found in %s:", directory.c_str());
            while ((file = readdir(dir)) != NULL) {
                std::string filename(file->d_name);

                std::size_t pos = filename.find(extension);
                if (pos == std::string::npos) {
                    continue;
                }

                filename.replace(pos, extension.size(), "");
                list.push_back(filename);
                LOG_V(TAG, filename.c_str());
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
