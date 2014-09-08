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

#include "Log.h"

#include "RoadBook.h"

#define TAG "RoadBook"

namespace RBook {

Error RoadBook::GetCurrentPoint(RoadPoint*& point) {
    Error ret;

    try {
        if (RoadPointList.empty()) {
            ret = ERROR_EMPTY_LIST;
        }
        else {
            point = RoadPointList[RoadPointIndex]; // We assume RoadPointIndex is properly calculated by the other functions.
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

Error RoadBook::GetDistanceFromPreviousPoint(float &distance) {
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

Error RoadBook::GetDistanceToNextPoint(float &distance) {
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

// Private

RoadBook::~RoadBook() {
    try {
        // Delete road point list
        while (!RoadPointList.empty()) {
            RoadPointList.pop_back();
        }

        RoadPointIndex = 0;
    }
    catch (std::exception& e) {
        // Do nothing
    }
}

Error RoadBook::Load() {
    Error ret;
    std::string roadbookcontent;

    // Load road book file
    LOG_I(TAG, "Load %s", FilePath.c_str());
    try {
        std::ifstream roadbookfile(FilePath.c_str());
        std::string mbc((std::istreambuf_iterator<char>(roadbookfile)),
                std::istreambuf_iterator<char>());
        roadbookcontent = mbc;
    }
    catch (std::ios_base::failure& e) {
        LOG_W(TAG, "Load: no such file!");
        ret = ERROR_NO_BOOK_FILE;
    }
    catch (std::exception& e) {
        LOG_E(TAG, "Load: generic failure!");
        ret = ERROR_FAIL;
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

    return ret;
}

Error RoadBook::ParseRoadBook(const std::string& roadbook) {
    Error ret;

    try {
        JSONNode mainnode = libjson::parse(roadbook);

        for (JSONNode::const_iterator i = mainnode.begin(); i != mainnode.end();
                ++i) {
            std::string nodename = i->name();

            if ((i->type() == JSON_STRING) && (nodename == "title")) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                Title = i->as_string();
            }
            else if ((i->type() == JSON_STRING)
                    && (nodename == "description")) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                Description = i->as_string();
            }
            else if ((i->type() == JSON_STRING) && (nodename == "location")) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                Location = i->as_string();
            }
            else if ((i->type() == JSON_ARRAY) && (nodename == "roadpoints")) {
                ret = ParseRoadPointList(*i);
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
        for (JSONNode::const_iterator i = roadpointlist.begin();
                i != roadpointlist.end(); ++i) {
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
        for (JSONNode::const_iterator i = roadpoint.begin();
                i != roadpoint.end(); ++i) {
            std::string nodename = i->name();

            if ((i->type() == JSON_STRING) && (nodename == "name")) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                rp->Name = i->as_string();
            }
            else if ((i->type() == JSON_STRING) && (nodename == "observation")) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                rp->Observation = i->as_string();
            }
            else if ((i->type() == JSON_NUMBER) && (nodename == "kilometer")) {
                LOG_V(TAG, "%s: %.2f", nodename.c_str(), i->as_float());
                rp->Kilometer = i->as_float();
            }
            else if ((i->type() == JSON_STRING) && (nodename == "type")) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                rp->Type = i->as_string();
            }
            else if ((i->type() == JSON_STRING) && (nodename == "direction")) {
                LOG_V(TAG, "%s: %s", nodename.c_str(), i->as_string().c_str());
                rp->Direction = i->as_string();
            }
        }

        // Add road point to the list
        if (rp != NULL) {
            LOG_V(TAG, "New point added: %s", rp->Name.c_str());
            RoadPointList.push_back(rp);
        }
    }
    catch (std::exception& e) {
        ret = RoadBook::ERROR_MAL_FORMATTED_BOOK;
    }

    return ret;
}

} // namespace

