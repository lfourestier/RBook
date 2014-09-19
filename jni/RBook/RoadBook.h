/*
 * RoadBook.h
 *
 *  Created on: Aug 26, 2014
 *      Author: Luc Fourestier
 */

#ifndef ROADBOOK_H_
#define ROADBOOK_H_

#include <string>
#include <vector>

#include "RoadPoint.h"
#include "Error.h"
#include "libjson.h"

#define ROADBOOK_ERROR_BASE CONFIG_ROADBOOK_ERROR_BASE

//! The road book file extension
#define ROADBOOK_EXTENSION ".mrb"

//! The compressed road book file extension
#define ROADBOOK_COMPRESSED_EXTENSION ".mrz"

namespace RBook {

class RoadBook {
    friend class BookManager;
public:

    //! RoadBook Error list
    enum ERROR {
        ERROR_BOOK_NOT_FOUND = ROADBOOK_ERROR_BASE, //!< Did not find the book specified.
        ERROR_MAL_FORMATTED_BOOK, //!< Roadbook file is mal formatted.
        ERROR_EMPTY_LIST, //!< Road point list is empty.
        ERROR_REACHED_END, //!< Reached the end of the road point list while navigating into it.
        ERROR_REACHED_START, //!< Reached the start of the road point list while navigating into it.
        ERROR_CANNOT_SAVE, //!< File issue while saving.
    };

    //! bookname (Short name for the book = file name w/o extension and path.)
    std::string Bookname;

    //! Road book title
    std::string Title;

    //! Road book general description
    std::string Description;

    //! Road book global location
    std::string Location;

    //! Road book total distance
    float TotalDistance;

    /**
     * File location where to save and load.
     */
    std::string FilePath;

    /**
     * Get the total number of points in the book.
     *
     * @param count: the number of points
     * @return @see ERROR
     */
    Error GetPointCount(unsigned int &count);

    /**
     * Get the current road point in the book.
     *
     * @param Point: The created RoadPoint object.
     * @return @see ERROR
     */
    Error GetCurrentPoint(RoadPoint*& point);

    /**
     * Get the next road point in the book.
     *
     * @param Point: The created RoadPoint object or NULL if at the end.
     * @return @see ERROR
     */
    Error GetNextPoint(RoadPoint*& point);

    /**
     * get the previous road point in the book.
     *
     * @param Point: The created RoadPoint object or NULL if at the beginning
     * @return @see ERROR
     */
    Error GetPreviousPoint(RoadPoint*& point);

    /**
     * Calculate the distance from the previous point in the list.
     *
     * @param distance: The distance
     * @return @see ERROR
     */
    Error GetDistanceFromPrevious(float &distance);

    /**
     * Calculate the distance to the next point in the list.
     *
     * @param distance: The distance
     * @return @see ERROR
     */
    Error GetDistanceToNext(float &distance);

    /**
     * Insert a new point into the list before the current position.
     * Create the point.
     * Current position point and next ones will be pushed.
     *
     * @param point: Return created point.
     * @return @see ERROR
     */
    Error AddNewPointBefore(RoadPoint*& point);

    /**
     * Insert a new point into the list after the current position.
     * Create the point.
     * Next points will be pushed. Index is incremented.
     *
     * @param point: returned new created point.
     * @return @see ERROR
     */
    Error AddNewPointAfter(RoadPoint*& point);

    /**
     * Delete current point.
     * Index will be updated to previous point if end of list, else remains as it is.
     *
     * @return @see ERROR
     */
    Error DeleteCurrentPoint();

    /**
     * Advance into the list.
     * Stays at end if end is reached and return REACHED_END.
     *
     * @return @see ERROR
     */
    Error Next();

    /**
     * Go back into the list.
     * Stays at beginning if start is reached and return REACHED_START.
     *
     * @return @see ERROR
     */
    Error Previous();

    /**
     * Reset road book to start point.
     *
     * @return @see ERROR
     */
    Error Reset();

    /**
     * Go to end of point list.
     *
     * @return @see ERROR
     */
    Error End();

private:
    /**
     * List of road points
     */
    std::vector<RoadPoint*> RoadPointList;

    /**
     * The current road point index.
     */
    unsigned int RoadPointIndex;

    //! Temporary directory where archive will be inflated.
    std::string TempArchiveDirectory;

    /**
     * Constructor.
     */
    RoadBook();

    /**
     * Destructor.
     */
    ~RoadBook();

    /**
     * Load the road book from the known location.
     * Initialize to the first point of the list.
     *
     * @return @see ERROR
     */
    Error Load();

    /**
     * Save the road book into the known location.
     *
     * @return @see ERROR
     */
    Error Save();

    /**
     * Delete the roadbook. Remove any corresponding resources (Files, directories...).
     *
     * @return @see ERROR
     */
    Error Delete();

    /**
     * Parse the general data of the road book
     *
     * @param content: The road book content
     * @return @see ERROR.
     */
    Error ParseRoadBook(const std::string& content);

    /**
     * Parse the road point list of the road book and create the object list out of it.
     *
     * @param roadpoints: The list of road points
     * @return @see ERROR
     */
    Error ParseRoadPointList(const JSONNode& pointpointlist);

    /**
     * Parse a road point and add it into the list.
     *
     * @param point: A road point
     * @return @see ERROR
     */
    Error ParseRoadPoint(const JSONNode& roadpoint);

    /**
     * Generate the road book json data.
     *
     * @param content: the returned content string.
     * @return @see ERROR
     */
    Error GenerateRoadBook(std::string& content);

    /**
     * Generate the road point json array.
     *
     * @param roadpoints: json array node where to store the array
     * @return @see ERROR
     */
    Error GenerateRoadPoints(JSONNode& roadpoints);

};

} // namespace

#endif /* ROADBOOK_H_ */
