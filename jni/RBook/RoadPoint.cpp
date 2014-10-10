/*
 * RoadPoint.cpp
 *
 *  Created on: Aug 26, 2014
 *      Author: Luc Fourestier
 */

#include "FileUtils.h"
#include "Log.h"

#include "RoadPoint.h"

#define TAG "RoadPoint"

namespace RBook {

RoadPoint::RoadPoint(std::string archivedirectory) {
    Description = "";
    Type = "";
    Kilometer = 0.0;
    Direction = "";
    Number = 0;
    TempArchiveDirectory = archivedirectory;
}

Error RoadPoint::GetImage(std::string &imagepath) {
    Error ret;

    if (Image.compare(ROADPOINT_NO_IMAGE) != 0) {
        imagepath = TempArchiveDirectory + FILEUTILS_PATH_DELIMITER + Image;
    }
    else {
        imagepath.clear();
    }

    return ret;
}

Error RoadPoint::SetImage(std::string imagepath){
    Error ret;

    if (imagepath.empty()) {
        Image = ROADPOINT_NO_IMAGE;
    }
    else {
        std::string path(imagepath);
        std::string name(imagepath);

        ret = FileUtils::GetFileName(name);
        if (ret != ERROR_OK) {
            return ERROR_FAIL;
        }

        // Check if the picture is already in the temp archive directory, if not copy the image
        ret = FileUtils::GetFilePath(path);
        if (ret != ERROR_OK) {
            return ERROR_FAIL;
        }

        if (path.compare(TempArchiveDirectory) != 0) {
            LOG_V(TAG, "Copy point image %s into %s", imagepath.c_str(), (TempArchiveDirectory + FILEUTILS_PATH_DELIMITER + name).c_str());
            ret = FileUtils::CopyFile(imagepath, TempArchiveDirectory + FILEUTILS_PATH_DELIMITER + name);
            if (ret != ERROR_OK) {
                return ERROR_FAIL;
            }
        }

        // Set the image field
        Image = name;
    }

    return ret;
}

} // namespace

