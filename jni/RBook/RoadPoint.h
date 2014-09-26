/*
 * RoadPoint.h
 *
 *  Created on: Aug 26, 2014
 *      Author: Luc Fourestier
 */

#ifndef ROADPOINT_H_
#define ROADPOINT_H_

#include <string>

#include "Error.h"

#define ROADPOINT_ERROR_BASE CONFIG_ROADBOOK_ERROR_BASE

// The json fields
#define ROADPOINT_DESCRIPTION_TAG "description"
#define ROADPOINT_KILOMETER_TAG "kilometer"
#define ROADPOINT_TYPE_TAG "type"
#define ROADPOINT_DIRECTION_TAG "direction"

namespace RBook {

class RoadPoint {
public:
    //! RoadPoint Error list
    enum ERROR {
        ERROR_NO_NAME = ROADPOINT_ERROR_BASE,
    };

    //! Road point name (Global name).
    std::string Description;

    //! Total distance.
    float Kilometer;

    //! Road point type (Cross point or round about...). Directly mapped to icon file.
    std::string Type;

    //! Description of the direction towards the next point (City, well known places, road numbers...).
    std::string Direction;

    //! The number of the point in the list.
    unsigned int Number;

    /**
     * Constructor.
     */
    RoadPoint();
};

} // namespace

#endif /* ROADPOINT_H_ */
