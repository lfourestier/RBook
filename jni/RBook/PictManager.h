/*
 * PictManager.h
 *
 *  Created on: Sep 26, 2014
 *      Author: befourel
 */

#ifndef PICTMANAGER_H_
#define PICTMANAGER_H_

#include <string>
#include <list>

#include "Error.h"

#define PICTMANAGER_ERROR_BASE CONFIG_PICTMANAGER_ERROR_BASE

namespace RBook {

/**
 * @brief Manages and organizes the picts used in the road book.
 * The pict manager inflate the pict archive file into the pict directory and then parse and list the available picts.
 * It provides access functions to get pict path as well.
 * @note A pict is the picture that represents the road point and the direction to take.
 */
class PictManager {
public:
    //! Error list
    enum ERROR {
        ERROR_NOT_INITIALIZED = PICTMANAGER_ERROR_BASE, //!< Did not find the pict file specified.
        ERROR_CANNOT_INFLATE, //!< Archive issue while inflating.
        ERROR_PICT_NOT_FOUND, //!< Could not find the pict.
    };

    /**
     * The different sizes in which the pict manager can provide the picts.
     * @notes If you add an extra resolution here. DO NOT forget to adapt @ref ResolutionLabels.
     */
    enum Resolution {
        RESOLUTION_FULL = 0, //!< The max available size of a pict. Do not remove!
        // INSERT NEW HERE!
        RESOLUTION_SMALL, //!< The smallest available size of a pict. Do not remove!
    };

    /**
     * Default constructor.
     */
    PictManager(){};

    /**
     * Populate the pict directory with the picts from archive.
     * @param archivepath: the path to the archive of picts to be inflated. File MUST exits.
     * @param rootdir: The directory where to inflate.
     * @return @see ERROR
     */
    Error Populate(std::string archivepath, std::string rootdir);


    /**
     * Get the list of pict available.
     * @param list: Returned list, composed of short name of picts.
     * @return @see ERROR
     */
    Error GetPictList(std::list<std::string> &list);

    /**
     * Get a pict file path corresponding to the requested resolution.
     * @param pict: The pict to retrieve.
     * @param resolution: The requested resolution (@see Resolution).
     * @param path: The returned path.
     * @return @see ERROR
     */
    Error GetPict(std::string pict, int resolution, std::string &path);

    /**
     * Get the speech string linked to the pict. (see ".speech" files in the pict archive).
     * @param pict: The pict
     * @param speech: the returned speech string
     * @return @see ERROR
     */
    Error GetPictSpeech(std::string pict, std::string &speech);


    /**
     * Destructor
     */
    ~PictManager();

private:
    //! The root directory where picts are inflated and stored (Full path).
    std::string PictDirectory;

    //! The list of picts.
    std::list<std::string> ListOfPicts;

    //! Corresponding resolution labels (@see Resolution). Used to get the proper pict file name.
    static const std::string ResolutionLabels[];

};

} /* namespace RBook */
#endif /* PICTMANAGER_H_ */
