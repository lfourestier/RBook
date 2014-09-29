/*
 * PictManager.cpp
 *
 *  Created on: Sep 26, 2014
 *      Author: befourel
 */

#include "FileUtils.h"
#include "Log.h"
#include "Archive.h"

#include "PictManager.h"

#define PICTMANAGER_DIR "/Pict"
#define PICTMANAGER_PICT_EXTENSION ".png"

#define TAG "PictManager"

namespace RBook {

const std::string PictManager::ResolutionLabels[] = {
            "_full", // DO NOT REMOVE
            // INSERT NEW HERE!
            "_small" // DO NOT REMOVE
    };

Error PictManager::Populate(std::string archivepath, std::string rootdir){
    Error ret;

    // Create pict directory
    PictDirectory = rootdir + PICTMANAGER_DIR;

    if (FileUtils::DirectoryExists(PictDirectory) == ERROR_OK) {
        FileUtils::RemoveDirectory(PictDirectory);
    }
    ret = FileUtils::MakeDirectory(PictDirectory);
    if(ret != ERROR_OK) {
        PictDirectory.clear();
        return ERROR_CANNOT_INFLATE;
    }

    // Populate directory from pict archive
    Archive pictarchive(archivepath);

    ret = pictarchive.Inflate(PictDirectory);
    if (ret != ERROR_OK) {
        LOG_E(TAG, "could not inflate pict archive!");
        return ERROR_CANNOT_INFLATE;
    }

    // Initialize the pict list
    ret = FileUtils::ListFilesRootInDir(PictDirectory, ResolutionLabels[0] + PICTMANAGER_PICT_EXTENSION, ListOfPicts);
    if ((ret != ERROR_OK ) || (ListOfPicts.empty())) {
        LOG_E(TAG, "could not initialize the pict list!");
        return ERROR_CANNOT_INFLATE;
    }

    return ret;
}

Error PictManager::GetPictList(std::list<std::string> &list){
    Error ret;

    try {
        if (!ListOfPicts.empty()) {
            // Copy list
             for (std::list<std::string>::iterator i=ListOfPicts.begin(); i != ListOfPicts.end(); ++i) {
                 list.push_front(*i);
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

Error PictManager::GetPict(std::string pict, int resolution, std::string &path){
    Error ret;

    try {
        if (!ListOfPicts.empty()) {
            ret = ERROR_PICT_NOT_FOUND;

            for (std::list<std::string>::iterator i=ListOfPicts.begin(); i != ListOfPicts.end(); ++i) {
                if (pict.compare(*i) == 0) {
                    std::string temppath = PictDirectory + FILEUTILS_PATH_DELIMITER + pict + ResolutionLabels[resolution] + PICTMANAGER_PICT_EXTENSION;
                    LOG_V(TAG, "Found pict for %s: %s", pict.c_str(), temppath.c_str());
                    if (FileUtils::FileExists(temppath) == ERROR_OK) {
                        path = temppath;
                    }
                    else {
                        LOG_E(TAG, "Cannot find pict %s", pict.c_str());
                        ret = ERROR_PICT_NOT_FOUND;
                    }
                    ret = ERROR_OK;
                    break;
                }
            }
        }
        else {
            LOG_E(TAG, "Pict list is empty");
            ret = ERROR_PICT_NOT_FOUND;
        }
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

PictManager::~PictManager() {
    try {
        // Delete pict list
        while (!ListOfPicts.empty()) {
            ListOfPicts.pop_back();
        }
        // Delete pict directory
        if (FileUtils::DirectoryExists(PictDirectory) == ERROR_OK) {
            FileUtils::RemoveDirectory(PictDirectory);
            PictDirectory.clear();
        }
    }
    catch (std::exception& e) {
        // Do nothing
    }
}

} /* namespace RBook */
