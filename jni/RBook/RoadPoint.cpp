/*
 * RoadPoint.cpp
 *
 *  Created on: Aug 26, 2014
 *      Author: Luc Fourestier
 */

#include <sstream>
#include "FileUtils.h"
#include "Log.h"

#include "RoadPoint.h"

#define ROADPOINT_IMAGE_FILE_ROOT "point"
#define ROADPOINT_IMAGE_FILE_EXT ".png"

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

    if ((Image.compare(ROADPOINT_NO_IMAGE) != 0) && !TempArchiveDirectory.empty()) {
        imagepath = TempArchiveDirectory + FILEUTILS_PATH_DELIMITER + Image;
    }
    else {
        imagepath.clear();
    }

    return ret;
}

Error RoadPoint::CreateImagePath(std::string &imagepath) {
    Error ret;

    if (!TempArchiveDirectory.empty()) {
        std::stringstream strstream;
        strstream << Number;
        imagepath = TempArchiveDirectory + FILEUTILS_PATH_DELIMITER + ROADPOINT_IMAGE_FILE_ROOT + strstream.str() + ROADPOINT_IMAGE_FILE_EXT;
    }
    else {
        imagepath.clear();
        ret = ERROR_FAIL;
    }

    return ret;
}

Error RoadPoint::SetImage(std::string imagepath){
    Error ret;

    if (imagepath.empty() || TempArchiveDirectory.empty()) {
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

