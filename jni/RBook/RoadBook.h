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

#define ROADBOOK_ERROR_BASE 1000

namespace RBook {

class RoadBook {
    friend class BookManager;
public:

    //! RoadBook Error list
    enum ERROR {
        ERROR_NO_BOOK_FILE = ROADBOOK_ERROR_BASE,
        ERROR_MAL_FORMATTED_BOOK,
        ERROR_EMPTY_LIST,
        ERROR_REACHED_END,
        ERROR_REACHED_START,
    };

    //! Road book title
    std::string Title;

    //! Road book general description
    std::string Description;

    //! Road book global location
    std::string Location;

    //! Road book total distance
    float TotalDistance;

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
    Error GetDistanceFromPreviousPoint(float &distance);

    /**
     * Calculate the distance to the next point in the list.
     *
     * @param distance: The distance
     * @return @see ERROR
     */
    Error GetDistanceToNextPoint(float &distance);

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

private:
    /**
     * File location where to save and load.
     */
    std::string FilePath;

    /**
     * List of road points
     */
    std::vector<RoadPoint*> RoadPointList;

    /**
     * The current road point index.
     */
    unsigned int RoadPointIndex;

    /**
     * Constructor.
     */
    RoadBook() :
            RoadPointIndex(0) {
    }
    ;

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
     * Parse the general data of the road book
     *
     * @param motobook: The road book
     * @return @see ERROR.
     */
    Error ParseRoadBook(const std::string& roadbook);

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

};

} // namespace

#endif /* ROADBOOK_H_ */
