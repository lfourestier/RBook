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
#include <sys/stat.h>

#include "Log.h"

#include "RoadBook.h"

#define TAG "RoadBook"

#define ROADBOOK_TITLE_TAG "title"
#define ROADBOOK_DESCRIPTION_TAG "description"
#define ROADBOOK_LOCATION_TAG "location"
#define ROADBOOK_ROADPOINTS_TAG "roadpoints"

#define ROADPOINT_DESCRIPTION_TAG "description"
#define ROADPOINT_KILOMETER_TAG "kilometer"
#define ROADPOINT_TYPE_TAG "type"
#define ROADPOINT_DIRECTION_TAG "direction"

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
        ret = ERROR_OK;
        return ret;
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
        ret = ERROR_OK;
        return ret;
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

Error RoadBook::AddNewPointBefore(RoadPoint*& point) {
    Error ret;

    try {
        point = new RoadPoint();
        if (RoadPointList.empty()) {
            RoadPointList.push_back(point);
        }
        else {
            std::vector<RoadPoint*>::iterator iterator = RoadPointList.begin();
            std::advance(iterator, RoadPointIndex);
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
        point = new RoadPoint();
        if (RoadPointList.empty() || (RoadPointIndex+1 >= RoadPointList.size())) {
            RoadPointList.push_back(point);
        }
        else {
            std::vector<RoadPoint*>::iterator iterator = RoadPointList.begin();
            std::advance(iterator, RoadPointIndex + 1);
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

RoadBook::RoadBook() : RoadPointIndex(0) {
    Title = "";
    Description = "";
    Location = "";
    TotalDistance = 0.0;
}

RoadBook::~RoadBook() {
    try {
        // Delete road point list
        while (!RoadPointList.empty()) {
            RoadPointList.pop_back();
        }

        // If loaded, release any other created resources (Files...)
        // TODO delete any uncompressed resources here (.mrz functionality)

        RoadPointIndex = 0;
    }
    catch (std::exception& e) {
        // Do nothing
    }
}

Error RoadBook::Load() {
    Error ret;
    std::string roadbookcontent;

    if ((!FilePath.empty()) && (FileExists(FilePath) == ERROR_OK)) {
        // TODO Insert .mrz (compressed roadbooks) functionality here.

        // Load road book file
        LOG_I(TAG, "Load %s", FilePath.c_str());
        try {
            std::ifstream roadbookfile(FilePath.c_str());
            std::string mbc((std::istreambuf_iterator<char>(roadbookfile)), std::istreambuf_iterator<char>());
            roadbookcontent = mbc;
        }
        catch (std::ios_base::failure& e) {
            LOG_W(TAG, "Load: no such file!");
            ret = ERROR_FILE_NOT_FOUND;
        }
        catch (std::exception& e) {
            LOG_E(TAG, "Load: generic failure!");
            ret = ERROR_FAIL;
        }
    }
    else {
        ret = ERROR_FILE_NOT_FOUND;
    }

    if (ret == ERROR_OK) {
        // Default
        Title = "";
        Description = "";
        Location = "";
        TotalDistance = 0.0;

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
            std::ofstream file(FilePath.c_str(), std::ofstream::trunc);
            std::string content;
            ret = GenerateRoadBook(content);
            if (ret == ERROR_OK) {
                file << content;
            }
            file.close();
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
        ret = ERROR_FILE_NOT_FOUND;
    }

    return ret;
}

Error RoadBook::Delete() {
    Error ret;

    if ((!FilePath.empty()) && (FileExists(FilePath) == ERROR_OK)) {
        if (remove(FilePath.c_str()) != 0) {
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
            else if ((i->type() == JSON_ARRAY) && (nodename == ROADBOOK_ROADPOINTS_TAG)) {
                ret = ParseRoadPointList(*i); // TODO Crash if array is empty. Why?
            }
        }

        TotalDistance = RoadPointList.back()->Kilometer; // Last point kilometer.
    }
    catch (std::exception& e) {
        ret = RoadBook::ERROR_MAL_FORMATTED_BOOK;
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
        ret = RoadBook::ERROR_MAL_FORMATTED_BOOK;
    }

    return ret;
}

Error RoadBook::ParseRoadPoint(const JSONNode& roadpoint) {
    Error ret;
    RoadPoint *rp = new RoadPoint();

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
        }

        // Add road point to the list
        if (rp != NULL) {
            LOG_V(TAG, "New point added: %s", rp->Description.c_str());
            RoadPointList.push_back(rp);
        }
    }
    catch (std::exception& e) {
        ret = RoadBook::ERROR_MAL_FORMATTED_BOOK;
    }

    return ret;
}

Error RoadBook::GenerateRoadBook(std::string& content) {
    Error ret;

    try {
        JSONNode mainnode(JSON_NODE);

        mainnode.push_back(JSONNode(ROADBOOK_TITLE_TAG, Title));
        mainnode.push_back(JSONNode(ROADBOOK_DESCRIPTION_TAG, Description));
        mainnode.push_back(JSONNode(ROADBOOK_LOCATION_TAG, Location));

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

            roadpointnode.push_back(JSONNode(ROADPOINT_DESCRIPTION_TAG, RoadPointList[index]->Description));
            roadpointnode.push_back(JSONNode(ROADPOINT_KILOMETER_TAG, RoadPointList[index]->Kilometer));
            roadpointnode.push_back(JSONNode(ROADPOINT_TYPE_TAG, RoadPointList[index]->Type));
            roadpointnode.push_back(JSONNode(ROADPOINT_DIRECTION_TAG, RoadPointList[index]->Direction));

            roadpoints.push_back(roadpointnode);
        }
    }
    catch (std::exception& e) {
        ret = ERROR_FAIL;
    }

    return ret;
}

Error RoadBook::FileExists(std::string file) {
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

} // namespace

