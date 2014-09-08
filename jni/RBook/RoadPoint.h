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

#define ROADPOINT_ERROR_BASE 2000

namespace RBook {

class RoadPoint {
public:
    //! RoadPoint Error list
    enum ERROR {
        ERROR_NO_NAME = ROADPOINT_ERROR_BASE,
    };

    //! Road point name (Global name).
    std::string Name;

    //! Road point observations.
    std::string Observation;

    //! Total distance.
    float Kilometer;

    //! Road point type (Cross point or round about...). Directly mapped to icon file.
    std::string Type;

    //! Description of the direction towards the next point (City, well known places, road numbers...).
    std::string Direction;

    /**
     * Constructor.
     */
    RoadPoint();
};

} // namespace

#endif /* ROADPOINT_H_ */
