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

#define ROADPOINT_ERROR_BASE ERROR_ROADPOINT_BASE

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
