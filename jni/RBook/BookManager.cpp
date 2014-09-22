/*
 * BookManager.cpp
 *
 *  Created on: Sep 1, 2014
 *      Author: Luc Fourestier
 */

#include "Log.h"
#include "FileUtils.h"

#include "BookManager.h"

//! The road book directory
#define BOOK_DIR "/RBook"

#define TAG "BookManager"

namespace RBook {

Error BookManager::Initialize(std::string rootdir) {
    Error ret;

    RBookDirectory = rootdir + BOOK_DIR;

    ret = FileUtils::DirectoryExists(RBookDirectory);
    if (ret == FileUtils::ERROR_DIR_NOT_FOUND) {
        LOG_D(TAG, "RBook dir does not exist.");
        ret = FileUtils::MakeDirectory(RBookDirectory);
    }
    if(ret != ERROR_OK) {
        RBookDirectory.clear();
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
        if (!RBookDirectory.empty()) {
            // Refresh list
            ret = FileUtils::ListFilesInDir(RBookDirectory, ROADBOOK_COMPRESSED_EXTENSION, ListOfBooks);

            if ((ret == ERROR_OK ) && (!ListOfBooks.empty())) {
                // Copy list
                 for (std::list<std::string>::iterator i=ListOfBooks.begin(); i != ListOfBooks.end(); ++i) {
                     booklist.push_front(*i);
                 }
             }
        }
        else {
            ret = ERROR_NOT_INITIALIZED;
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
                    if (roadbook != NULL) {
                        roadbook->FilePath = RBookDirectory + "/" + bookname + ROADBOOK_COMPRESSED_EXTENSION;
                        roadbook->Bookname = bookname;
                        ret = roadbook->Load();
                    }
                    else {
                        ret = ERROR_FAIL;
                    }
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

Error BookManager::CreateRoadBook(std::string bookname, RoadBook *& roadbook) {
    Error ret;

    roadbook = new RoadBook();
    if (roadbook != NULL) {
        roadbook->FilePath = RBookDirectory + "/" + bookname + ROADBOOK_COMPRESSED_EXTENSION;
        roadbook->Bookname = bookname;
        ret = roadbook->Create();
    }
    else {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error BookManager::SaveRoadBook(RoadBook *& roadbook) {
    Error ret;

    if (!roadbook->FilePath.empty()) {
        ret = roadbook->Save();
    }
    else {
        ret = ERROR_ROADBOOK_INVALID;
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

Error BookManager::DeleteRoadBook(std::string bookname) {
    Error ret;

    try {
        if (!ListOfBooks.empty()) {
            ret = ERROR_BOOKNAME_NOT_FOUND;

            for (std::list<std::string>::iterator i=ListOfBooks.begin(); i != ListOfBooks.end(); ++i) {
                if (bookname.compare(*i) == 0) {
                    RoadBook* roadbook = new RoadBook();
                    if (roadbook != NULL) {
                        roadbook->FilePath = RBookDirectory + "/" + bookname + ROADBOOK_COMPRESSED_EXTENSION;
                        ret = roadbook->Delete();
                        delete roadbook;
                    }
                    else {
                        ret = ERROR_FAIL;
                    }
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

} /* namespace RBook */
