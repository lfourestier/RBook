/*
 * RoadBook.cpp
 *
 *  Created on: Aug 26, 2014
 *      Author: Luc Fourestier
 */

#include <iostream>
#include <ios>
#include <fstream>
#include <streambuf>
#include <exception>
#include <iterator>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "Log.h"
#include "zip.h"
#include "unzip.h"
#include "Archive.h"
#include "FileUtils.h"

#include "RoadBook.h"

#define TAG "RoadBook"

//! The road book file
#define ROADBOOK_FILE "roadbook.mrb"
#define ROADBOOK_IMAGE_FILE "book.png"

#define ROADBOOK_SPEECH_UNIT "kilometers"
#define ROADBOOK_SPEECH_MILEAGE "mileage"

#define ROADBOOK_SPEECH_PARTIAL_DISTANCE "after %.2f " ROADBOOK_SPEECH_UNIT ", "
#define ROADBOOK_SPEECH_KILOMETER "at " ROADBOOK_SPEECH_MILEAGE " %.2f, "
#define ROADBOOK_SPEECH_DESCRIPTION " %s, "
#define ROADBOOK_SPEECH_ACTION " %s, "
#define ROADBOOK_SPEECH_DIRECTION "then %s."

#define ROADBOOK_SPEECH_NEXT_SENTENCE ROADBOOK_SPEECH_PARTIAL_DISTANCE ROADBOOK_SPEECH_KILOMETER ROADBOOK_SPEECH_DESCRIPTION ROADBOOK_SPEECH_ACTION ROADBOOK_SPEECH_DIRECTION
#define ROADBOOK_SPEECH_POINT_SENTENCE ROADBOOK_SPEECH_KILOMETER ROADBOOK_SPEECH_DESCRIPTION ROADBOOK_SPEECH_ACTION ROADBOOK_SPEECH_DIRECTION

#define ROADBOOK_SPEECH_BUFFER_SIZE 1024

#define ROADBOOK_NO_IMAGE ROADPOINT_NO_IMAGE
#define ROADBOOK_NO_TEXT ROADPOINT_NO_TEXT

namespace RBook {

Error RoadBook::GetPointCount(unsigned int &count) {
    Error ret;
    count = RoadPointList.size();
    return ret;
}

Error RoadBook::GetCurrentPoint(RoadPoint*& point) {
    Error ret;

    try {
        if (RoadPointList.empty()) {
            ret = ERROR_EMPTY_LIST;
        }
        else {
            point = RoadPointList[RoadPointIndex]; // We assume RoadPointIndex is properly calculated by the other functions.
            point->Number = RoadPointIndex+1;
        }
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error RoadBook::GetNextPoint(RoadPoint*& point) {
    Error ret;

    try {
        if (RoadPointList.empty()) {
            ret = ERROR_EMPTY_LIST;
        }
        else if (RoadPointIndex + 1 < RoadPointList.size()) {
            point = RoadPointList[RoadPointIndex + 1];
            point->Number = RoadPointIndex+2;
        }
        else {
            ret = ERROR_REACHED_END;
        }
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error RoadBook::GetPreviousPoint(RoadPoint*& point) {
    Error ret;

    try {
        if (RoadPointList.empty()) {
            ret = ERROR_EMPTY_LIST;
        }
        else if (RoadPointIndex > 0) {
            point = RoadPointList[RoadPointIndex - 1];
            point->Number = RoadPointIndex;
        }
        else {
            ret = ERROR_REACHED_START;
        }
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error RoadBook::GetDistanceFromPrevious(float &distance) {
    Error ret;
    RoadPoint* previouspoint;
    RoadPoint* currentpoint;

    ret = GetPreviousPoint(previouspoint);
    if (ret == ERROR_REACHED_START) {
        distance = 0.0;
        return ERROR_OK;
    }
    else if (ret != ERROR_OK) {
        return ret;
    }

    ret = GetCurrentPoint(currentpoint);
    if (ret != ERROR_OK) {
        return ret;
    }

    distance = currentpoint->Kilometer - previouspoint->Kilometer;

    return ret;
}

Error RoadBook::GetDistanceToNext(float &distance) {
    Error ret;
    RoadPoint* nextpoint;
    RoadPoint* currentpoint;

    ret = GetNextPoint(nextpoint);
    if (ret == ERROR_REACHED_END) {
        distance = 0.0;
        return ERROR_OK;
    }
    else if (ret != ERROR_OK) {
        return ret;
    }

    ret = GetCurrentPoint(currentpoint);
    if (ret != ERROR_OK) {
        return ret;
    }

    distance = nextpoint->Kilometer - currentpoint->Kilometer;

    return ret;
}

Error RoadBook::GetCurrentPointSpeech(PictManager *pictmgr, std::string &speech) {
    Error ret;

    RoadPoint *roadpoint = NULL;
    ret = GetCurrentPoint(roadpoint);
    if ((ret != ERROR_OK) || (roadpoint == NULL)) {
        return ERROR_FAIL;
    }

    std::string pictspeech(" ");
    if (pictmgr != NULL) {
        pictmgr->GetPictSpeech(roadpoint->Type, pictspeech);
    }

    char buffer[ROADBOOK_SPEECH_BUFFER_SIZE];
    int count = snprintf(buffer, ROADBOOK_SPEECH_BUFFER_SIZE, ROADBOOK_SPEECH_POINT_SENTENCE, roadpoint->Kilometer, roadpoint->Description.c_str(), pictspeech.c_str(), roadpoint->Direction.c_str());
    if (count <= 0) {
        return ERROR_FAIL;
    }
    speech = buffer;

    return ret;
}

Error RoadBook::GetNextPointSpeech(PictManager *pictmgr, std::string &speech) {
    Error ret;

    RoadPoint *roadpoint = NULL;
    ret = GetNextPoint(roadpoint);
    if ((ret != ERROR_OK) || (roadpoint == NULL)) {
        return ERROR_FAIL;
    }

    std::string pictspeech(" ");
    if (pictmgr != NULL) {
        pictmgr->GetPictSpeech(roadpoint->Type, pictspeech);
    }

    float distance = 0.0;
    ret = GetDistanceToNext(distance);
    if (ret != ERROR_OK) {
        distance = 0.0;
    }

    char buffer[ROADBOOK_SPEECH_BUFFER_SIZE];
    int count = snprintf(buffer, ROADBOOK_SPEECH_BUFFER_SIZE, ROADBOOK_SPEECH_NEXT_SENTENCE, distance, roadpoint->Kilometer, roadpoint->Description.c_str(), pictspeech.c_str(), roadpoint->Direction.c_str());
    if (count <= 0) {
        return ERROR_FAIL;
    }
    speech = buffer;

    return ret;
}

Error RoadBook::GetImage(std::string &imagepath) {
    Error ret;

    if ((Image.compare(ROADBOOK_NO_IMAGE) != 0) && !TempArchiveDirectory.empty()) {
        imagepath = TempArchiveDirectory + FILEUTILS_PATH_DELIMITER + Image;
    }
    else {
        imagepath.clear();
    }

    return ret;
}

Error RoadBook::CreateImagePath(std::string &imagepath) {
    Error ret;

    if (!TempArchiveDirectory.empty()) {
        imagepath = TempArchiveDirectory + FILEUTILS_PATH_DELIMITER + ROADBOOK_IMAGE_FILE;
        //Image = ROADBOOK_IMAGE_FILE;
    }
    else {
        imagepath.clear();
        ret = ERROR_FAIL;
    }

    return ret;
}

Error RoadBook::SetImage(std::string imagepath){
    Error ret;

    if (imagepath.empty() || TempArchiveDirectory.empty()) {
        Image = ROADBOOK_NO_IMAGE;
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
            LOG_V(TAG, "Copy image %s into %s", imagepath.c_str(), (TempArchiveDirectory + FILEUTILS_PATH_DELIMITER + name).c_str());
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

Error RoadBook::AddNewPointBefore(RoadPoint*& point) {
    Error ret;

    try {
        point = new RoadPoint(TempArchiveDirectory);
        if (RoadPointList.empty()) {
            point->Number = 1;
            RoadPointList.push_back(point);
        }
        else {
            std::vector<RoadPoint*>::iterator iterator = RoadPointList.begin();
            std::advance(iterator, RoadPointIndex);
            point->Number = RoadPointIndex + 1;
            RoadPointList.insert(iterator, point);
        }
    } catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error RoadBook::AddNewPointAfter(RoadPoint*& point){
    Error ret;

    try {
        point = new RoadPoint(TempArchiveDirectory);
        if (RoadPointList.empty())  {
            point->Number = 1;
            RoadPointList.push_back(point);
        }
        else if (RoadPointIndex+1 >= RoadPointList.size()) {
            RoadPointList.push_back(point);
            point->Number = RoadPointIndex + 2;
            Next(); // Go to the new point
        }
        else {
            std::vector<RoadPoint*>::iterator iterator = RoadPointList.begin();
            std::advance(iterator, RoadPointIndex + 1);
            point->Number = RoadPointIndex + 2;
            RoadPointList.insert(iterator, point);
            Next(); // Go to the new point
        }
    } catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error RoadBook::DeleteCurrentPoint() {
    Error ret;

    try {
        if (!RoadPointList.empty()) {
            std::vector<RoadPoint*>::iterator iterator = RoadPointList.begin();
            std::advance(iterator, RoadPointIndex);
            RoadPointList.erase(iterator);

            // Re-adjust index
            if (RoadPointList.size() == 0) {
                RoadPointIndex = 0;
            }
            else if (RoadPointIndex >= RoadPointList.size()) {
                RoadPointIndex = RoadPointList.size()-1;
            }
        }
        else {
            ret = ERROR_EMPTY_LIST;
        }
    } catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error RoadBook::Next() {
    Error ret;

    if (RoadPointList.empty()) {
        ret = ERROR_EMPTY_LIST;
    }
    else if (RoadPointIndex < RoadPointList.size() - 1) {
        RoadPointIndex++;
    }
    else {
        ret = ERROR_REACHED_END;
    }

    return ret;
}

Error RoadBook::Previous() {
    Error ret;

    if (RoadPointList.empty()) {
        ret = ERROR_EMPTY_LIST;
    }
    else if (RoadPointIndex > 0) {
        RoadPointIndex--;
    }
    else {
        ret = ERROR_REACHED_START;
    }

    return ret;
}

Error RoadBook::Reset() {
    Error ret;

    RoadPointIndex = 0;

    return ret;
}

Error RoadBook::End() {
    Error ret;

    RoadPointIndex = RoadPointList.size() - 1;

    return ret;
}

// Private

RoadBook::RoadBook(std::string filepath, std::string bookname, std::string tempdirectory) : RoadPointIndex(0) {
    Title = "";
    Description = "";
    Location = "";
    TotalDistance = 0.0;
    Image = "";

    FilePath = filepath;
    Bookname = bookname;
    TempArchiveDirectory = tempdirectory + FILEUTILS_PATH_DELIMITER + Bookname;
}

RoadBook::~RoadBook() {
    try {
        // Delete road point list
        while (!RoadPointList.empty()) {
            RoadPointList.pop_back();
        }

        RemoveTemporaryDirectory();

        RoadPointIndex = 0;
    }
    catch (std::exception& e) {
        // Do nothing
    }
}

Error RoadBook::Create() {
    Error ret;

    // Prepare a new road book
    if (!FilePath.empty()) {
        ret = CreateTemporaryDirectory();
        if (ret != ERROR_OK) {
            LOG_E(TAG, "could create archive temporary directory!");
            return ERROR_CANNOT_CREATE;
        }
    }
    else {
        ret = ERROR_CANNOT_CREATE;
    }

    return ret;
}

Error RoadBook::Load() {
    Error ret;
    std::string roadbookcontent;

    if ((!FilePath.empty()) && (FileUtils::FileExists(FilePath) == ERROR_OK)) {
        // Load road book file
        LOG_I(TAG, "Load %s", FilePath.c_str());
        try {
            Archive archive(FilePath);

            ret = CreateTemporaryDirectory();
            if (ret != ERROR_OK) {
                LOG_E(TAG, "could create archive temporary directory!");
                return ERROR_CANNOT_CREATE;
            }

            ret = archive.Inflate(TempArchiveDirectory);
            if (ret != ERROR_OK) {
                LOG_E(TAG, "could not inflate archive!");
                return ERROR_CANNOT_CREATE;
            }

            // Find the file to parse for the roadbook
            std::string mrbfilepath = TempArchiveDirectory + FILEUTILS_PATH_DELIMITER + ROADBOOK_FILE;
            LOG_D(TAG, "Parsing file: %s", mrbfilepath.c_str());

            // Parse the roadbook
            if (FileUtils::FileExists(mrbfilepath) == ERROR_OK) {
                std::ifstream roadbookfile(mrbfilepath.c_str());
                std::string mbc((std::istreambuf_iterator<char>(roadbookfile)), std::istreambuf_iterator<char>());
                roadbookcontent = mbc;
            }
            else {
                LOG_E(TAG, "%s not found!", mrbfilepath.c_str());
                ret = ERROR_BOOK_NOT_FOUND;
            }
        }
        catch (std::ios_base::failure& e) {
            LOG_W(TAG, "Load: no such file!");
            ret = ERROR_BOOK_NOT_FOUND;
        }
        catch (std::exception& e) {
            LOG_E(TAG, "Load: generic failure!");
            ret = ERROR_FAIL;
        }
    }
    else {
        ret = ERROR_BOOK_NOT_FOUND;
    }

    if (ret == ERROR_OK) {
         // Parse road book file
        ret = ParseRoadBook(roadbookcontent);
    }

    return ret;
}

Error RoadBook::Save() {
    Error ret;

    if (!FilePath.empty()) {
        // Save road book file
        LOG_I(TAG, "Save %s", FilePath.c_str());
        try {
            // Check if temporary directories have been created
            if (TempArchiveDirectory.empty()) {
                return ERROR_CANNOT_SAVE;
            }

            if (FileUtils::DirectoryExists(TempArchiveDirectory) == FileUtils::ERROR_DIR_NOT_FOUND) {
                ret = FileUtils::MakeDirectory(TempArchiveDirectory);
                if (ret != ERROR_OK) {
                    return ERROR_CANNOT_SAVE;
                }
            }

            // Create the file of the roadbook (.mrb)
            std::string mrbfilepath = TempArchiveDirectory + FILEUTILS_PATH_DELIMITER + ROADBOOK_FILE;

            std::ofstream file(mrbfilepath.c_str(), std::ofstream::trunc);
            std::string content;
            ret = GenerateRoadBook(content);
            if (ret == ERROR_OK) {
                file << content;
            }
            file.close();

            Archive archive(FilePath);
            ret = archive.Deflate(TempArchiveDirectory);
            if (ret != ERROR_OK) {
                LOG_E(TAG, "could not deflate archive!");
                return ERROR_CANNOT_SAVE;
            }

        }
        catch (std::ios_base::failure& e) {
            LOG_W(TAG, "Save: Could not save file!");
            ret = ERROR_CANNOT_SAVE;
        }
        catch (std::exception& e) {
            LOG_E(TAG, "Load: generic failure!");
            ret = ERROR_FAIL;
        }
    }
    else {
        ret = ERROR_BOOK_NOT_FOUND;
    }

    return ret;
}

Error RoadBook::Delete() {
    Error ret;

    ret = RemoveTemporaryDirectory();
    ret = FileUtils::DeleteFile(FilePath);

    return ret;
}

Error RoadBook::CreateTemporaryDirectory() {
    Error ret;

    // Create temporary directory where to inflate archive
    if (!TempArchiveDirectory.empty()) {
        if (FileUtils::DirectoryExists(TempArchiveDirectory) == ERROR_OK) {
            ret = FileUtils::RemoveDirectory(TempArchiveDirectory);
            if (ret != ERROR_OK) {
                LOG_W(TAG, "could not remove directory!");
                return ERROR_FAIL;
            }
        }

        ret = FileUtils::MakeDirectory(TempArchiveDirectory);
        if (ret != ERROR_OK) {
            return ERROR_FAIL;
        }
    }
    else {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error RoadBook::RemoveTemporaryDirectory() {
    Error ret;

    if (!TempArchiveDirectory.empty()) {
        ret = FileUtils::RemoveDirectory(TempArchiveDirectory);
        if (ret != ERROR_OK) {
            LOG_W(TAG, "could not remove directory!");
            ret = ERROR_FAIL;
        }
    }

    return ret;
}

Error RoadBook::ParseRoadBook(const std::string& content) {
    Error ret;

    try {
        JSONNode mainnode = libjson::parse(content);

        for (JSONNode::const_iterator i = mainnode.begin(); i != mainnode.end(); ++i) {
            std::string nodename = i->name();

            if ((i->type() == JSON_STRING) && (nodename == ROADBOOK_TITLE_TAG)) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                Title = i->as_string();
            }
            else if ((i->type() == JSON_STRING) && (nodename == ROADBOOK_DESCRIPTION_TAG)) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                Description = i->as_string();
            }
            else if ((i->type() == JSON_STRING) && (nodename == ROADBOOK_LOCATION_TAG)) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                Location = i->as_string();
            }
            else if ((i->type() == JSON_STRING) && (nodename == ROADBOOK_IMAGE_TAG)) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                Image = i->as_string();
            }
            else if ((i->type() == JSON_ARRAY) && (nodename == ROADBOOK_ROADPOINTS_TAG)) {
                ret = ParseRoadPointList(*i); // TODO Crash if array is empty. Why?
            }
        }

        TotalDistance = RoadPointList.back()->Kilometer; // Last point kilometer.
    }
    catch (std::exception& e) {
        ret = ERROR_MAL_FORMATTED_BOOK;
    }

    return ret;
}

Error RoadBook::ParseRoadPointList(const JSONNode& roadpointlist) {
    Error ret;

    try {
        for (JSONNode::const_iterator i = roadpointlist.begin(); i != roadpointlist.end(); ++i) {
            std::string nodename = i->name();

            if (i->type() == JSON_NODE) {
                LOG_V(TAG, "# Roadpoint #");
                ret = ParseRoadPoint(*i);
            }
        }
    }
    catch (std::exception& e) {
        ret = ERROR_MAL_FORMATTED_BOOK;
    }

    return ret;
}

Error RoadBook::ParseRoadPoint(const JSONNode& roadpoint) {
    Error ret;
    RoadPoint *rp = new RoadPoint(TempArchiveDirectory);

    try {
        // Parse JSON
        for (JSONNode::const_iterator i = roadpoint.begin(); i != roadpoint.end(); ++i) {
            std::string nodename = i->name();

            if ((i->type() == JSON_STRING) && (nodename == ROADPOINT_DESCRIPTION_TAG)) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                rp->Description = i->as_string();
            }
            else if ((i->type() == JSON_NUMBER) && (nodename == ROADPOINT_KILOMETER_TAG)) {
                LOG_V(TAG, "%s: %.2f", nodename.c_str(), i->as_float());
                rp->Kilometer = i->as_float();
            }
            else if ((i->type() == JSON_STRING) && (nodename == ROADPOINT_TYPE_TAG)) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                rp->Type = i->as_string();
            }
            else if ((i->type() == JSON_STRING) && (nodename == ROADPOINT_DIRECTION_TAG)) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                rp->Direction = i->as_string();
            }
            else if ((i->type() == JSON_STRING) && (nodename == ROADPOINT_IMAGE_TAG)) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                rp->Image = i->as_string();
            }
        }

        // Add road point to the list
        if (rp != NULL) {
            LOG_V(TAG, "New point added: %s", rp->Description.c_str());
            RoadPointList.push_back(rp);
        }
    }
    catch (std::exception& e) {
        ret = ERROR_MAL_FORMATTED_BOOK;
    }

    return ret;
}

Error RoadBook::GenerateRoadBook(std::string& content) {
    Error ret;

    try {
        JSONNode mainnode(JSON_NODE);

        if (Title.empty()) {
            Title = ROADBOOK_NO_TEXT;
        }
        mainnode.push_back(JSONNode(ROADBOOK_TITLE_TAG, Title));

        if (Description.empty()) {
            Description = ROADBOOK_NO_TEXT;
        }
        mainnode.push_back(JSONNode(ROADBOOK_DESCRIPTION_TAG, Description));

        if (Location.empty()) {
            Location = ROADBOOK_NO_TEXT;
        }
        mainnode.push_back(JSONNode(ROADBOOK_LOCATION_TAG, Location));

        if (Image.empty()) {
            Image = ROADBOOK_NO_IMAGE;
        }
        mainnode.push_back(JSONNode(ROADBOOK_IMAGE_TAG, Image));

        JSONNode roadpointarray(JSON_ARRAY);
        roadpointarray.set_name(ROADBOOK_ROADPOINTS_TAG);
        ret = GenerateRoadPoints(roadpointarray);
        if (ret == ERROR_OK) {
            mainnode.push_back(roadpointarray);
        }
        else {
            return ret;
        }

        content = mainnode.write_formatted();
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error RoadBook::GenerateRoadPoints(JSONNode& roadpoints) {
    Error ret;

    try {
        for (int index = 0; index < RoadPointList.size(); ++index) {
            JSONNode roadpointnode(JSON_NODE);

            if (RoadPointList[index]->Description.empty()) {
                RoadPointList[index]->Description = ROADBOOK_NO_TEXT;
            }
            roadpointnode.push_back(JSONNode(ROADPOINT_DESCRIPTION_TAG, RoadPointList[index]->Description));
            roadpointnode.push_back(JSONNode(ROADPOINT_KILOMETER_TAG, RoadPointList[index]->Kilometer));

            if (RoadPointList[index]->Type.empty()) {
                RoadPointList[index]->Type = ROADBOOK_NO_TEXT;
            }
            roadpointnode.push_back(JSONNode(ROADPOINT_TYPE_TAG, RoadPointList[index]->Type));

            if (RoadPointList[index]->Direction.empty()) {
                RoadPointList[index]->Direction = ROADBOOK_NO_TEXT;
            }
            roadpointnode.push_back(JSONNode(ROADPOINT_DIRECTION_TAG, RoadPointList[index]->Direction));

            if (RoadPointList[index]->Image.empty()) {
                RoadPointList[index]->Image = ROADBOOK_NO_IMAGE;
            }
            roadpointnode.push_back(JSONNode(ROADPOINT_IMAGE_TAG, RoadPointList[index]->Image));

            roadpoints.push_back(roadpointnode);
        }
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

} // namespace

