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
#define BOOKMANAGER_DIR "/RBook"

//! The road book temporary directory
#define BOOKMANAGER_TEMP_DIR "/Temp"

#define TAG "BookManager"

namespace RBook {

Error BookManager::Initialize(std::string rootdir) {
    Error ret;

    // Check RBook directory and cerate if needed.
    RBookDirectory = rootdir + BOOKMANAGER_DIR;

    ret = FileUtils::DirectoryExists(RBookDirectory);
    if (ret == FileUtils::ERROR_DIR_NOT_FOUND) {
        ret = FileUtils::MakeDirectory(RBookDirectory);
    }
    if(ret != ERROR_OK) {
        RBookDirectory.clear();
        return ERROR_FAIL;
    }

    // Prepare temporary directory
    RBookTempDirectory = RBookDirectory + BOOKMANAGER_TEMP_DIR;
    if (FileUtils::DirectoryExists(RBookTempDirectory) == ERROR_OK) {
        FileUtils::RemoveDirectory(RBookTempDirectory);
    }
    ret = FileUtils::MakeDirectory(RBookTempDirectory);
    if(ret != ERROR_OK) {
        RBookTempDirectory.clear();
        return ERROR_FAIL;
    }

    return ret;
}

Error BookManager::GetRoadBookList(std::list<std::string> &booklist)
{
    Error ret;

    try {
        if (!RBookDirectory.empty() && (FileUtils::DirectoryExists(RBookDirectory) != FileUtils::ERROR_DIR_NOT_FOUND)) {
            // Refresh list
            ret = FileUtils::ListFilesRootInDir(RBookDirectory, ROADBOOK_COMPRESSED_EXTENSION, ListOfBooks);

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
                    roadbook = new RoadBook(RBookDirectory + FILEUTILS_PATH_DELIMITER + bookname + ROADBOOK_COMPRESSED_EXTENSION, bookname, RBookTempDirectory);
                    if (roadbook != NULL) {
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

    roadbook = new RoadBook(RBookDirectory + FILEUTILS_PATH_DELIMITER + bookname + ROADBOOK_COMPRESSED_EXTENSION, bookname, RBookTempDirectory);
    if (roadbook != NULL) {
        ret = roadbook->Create();
    }
    else {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error BookManager::ImportRoadBook(std::string filepath, std::string bookname, bool overwrite, RoadBook *& roadbook) {
    Error ret;

    try {
        // Check if book exists in the list
        if (!ListOfBooks.empty()) {
            for (std::list<std::string>::iterator i=ListOfBooks.begin(); i != ListOfBooks.end(); ++i) {
                if (bookname.compare(*i) == 0) {
                    ret = ERROR_ROADBOOK_EXISTS;
                    break;
                }
            }
        }

        if (ret == ERROR_ROADBOOK_EXISTS) {
            if (overwrite == true) {
                ret = DeleteRoadBook(bookname);
                if (ret != ERROR_OK) {
                    return ret;
                }
            }
            else {
                return ret;
            }
        }

        // Copy/overwrite file into RBook dir.
        std::string newfilename = RBookDirectory + FILEUTILS_PATH_DELIMITER + bookname + ROADBOOK_COMPRESSED_EXTENSION;
        ret = FileUtils::CopyFile(filepath, newfilename);
        if (ret != ERROR_OK) {
            return ERROR_FAIL;
        }

        // Open roadbook
        roadbook = new RoadBook(newfilename, bookname, RBookTempDirectory);
        if (roadbook != NULL) {
            ret = roadbook->Load();
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
                    RoadBook* roadbook = new RoadBook(RBookDirectory + FILEUTILS_PATH_DELIMITER + bookname + ROADBOOK_COMPRESSED_EXTENSION, bookname, RBookTempDirectory);
                    if (roadbook != NULL) {
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
        // Delete temporary files
        if (FileUtils::DirectoryExists(RBookTempDirectory) == ERROR_OK) {
            FileUtils::RemoveDirectory(RBookTempDirectory);
            RBookTempDirectory.clear();
        }
    }
    catch (std::exception& e) {
        // Do nothing
    }
}

} /* namespace RBook */
