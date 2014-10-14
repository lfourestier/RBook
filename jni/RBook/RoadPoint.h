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
#define ROADPOINT_IMAGE_TAG "image"

#define ROADPOINT_NO_IMAGE "none"
#define ROADPOINT_NO_TEXT " "

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

    //! the image name linked to the point.
    std::string Image;

    //! The number of the point in the list.
    unsigned int Number;

    /**
     * Constructor.
     * @param archivedirectory: Temporary road book directory
     */
    RoadPoint(std::string archivedirectory);

    /**
     * Get the image for the point.
     * @param imagepath: Full image path
     * @return @see ERROR
     */
    Error GetImage(std::string &imagepath);

    /**
     * Create an image path that application can use to store the image while editing.
     * @param imagepath: The imagepath or empty if it was not possible.
     * @return @see ERROR
     */
    Error CreateImagePath(std::string &imagepath);

    /**
     * Copy and set the image of the road point.
     * If the image is already in TempArchiveDirectory, then the copy is skipped.
     * @param imagepath: Image to copy (Full path)
     * @return @see ERROR
     */
    Error SetImage(std::string imagepath);

private:

    //! Temporary directory where archive is inflated.
   std::string TempArchiveDirectory;

    /**
     * Constructor.
     */
    RoadPoint(){};

};

} // namespace

#endif /* ROADPOINT_H_ */
