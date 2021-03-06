/*
 * BookManager.h
 *
 *  Created on: Sep 1, 2014
 *      Author: Luc Fourestier
 */

#ifndef BOOKMANAGER_H_
#define BOOKMANAGER_H_

#include <string>
#include <list>
#include <map>

#include "Error.h"
#include "RoadBook.h"

#define BOOKMANAGER_ERROR_BASE CONFIG_BOOKMANAGER_ERROR_BASE

namespace RBook {

/**
 * Manage the list of books and their locations.
 * Users only see "aliases" (called bookname) ide, a simple string representing the book.
 */
class BookManager {
public:

    //! BookManager Error list
    enum ERROR {
        ERROR_NOT_INITIALIZED = BOOKMANAGER_ERROR_BASE, //!< Directory not specified.
        ERROR_EMPTY_LIST, //!< List of file is empty
        ERROR_BOOKNAME_NOT_FOUND, //!< Did not find the book name in the list
        ERROR_ROADBOOK_INVALID, //!< The roadbook in use is somehow erroneous
        ERROR_ROADBOOK_EXISTS, //!< The imported roadbook already exits.
    };

    /**
     * Initialize the application root directory. The book will then be stored/read in/from "<rootdir>/BOOK_DIR" (@see BOOK_DIR).
     * Then:
     * - If directory contains books, it creates a list of those books.
     * - If directory is empty, the list is empty.
     * - If directory does not exist, it creates it (List is then empty!).
     *
     * @pre <rootdir> MUST exist
     *
     * @example If <rootdir>="/sdcard" and RBOOK_DIR="RBook", "/sdcard/RBook" will be created. Books will be then placed here.
     *
     * @param rootdir: The root directory
     * @return @see ERROR
     */
    Error Initialize(std::string rootdir);

    /**
     * Get teh book directory or ERROR is not created yet
     * @param dir: The returned full directory path
     * @return @see ERROR
     */
    Error GetBookDir(std::string &dir);

    /**
     * Return the book list.
     *
     * @param booklist: The list of bookname
     * @return @see ERROR
     */
    Error GetRoadBookList(std::list<std::string> &booklist);

    /**
     * Get the road book corresponding to the bookname.
     *
     * @param bookname: the bookname
     * @param roadbook: the returned roadbook
     * @return @see ERROR
     */
    Error GetRoadBook(std::string bookname, RoadBook *& roadbook);

    /**
     * Create a new blank road book.
     *
     * @param bookname: the bookname
     * @param roadbook: the returned roadbook
     * @return @see ERROR
     */
    Error CreateRoadBook(std::string bookname, RoadBook *& roadbook);

    /**
     * Import an external roadbook into the book list.
     * If roadbook already exist in the list and overwrite is false => return an error.
     * If roadbook already exist in the list and overwrite is true => Delete existing roadbook and copy the new one.
     *
     * @param filepath: The path of the file to import.
     * @param bookname: the bookname
     * @param overwrite: If true and bookname exists in the list, overwrite the book in the list of books.
     * @param roadbook: the returned roadbook
     * @return @see ERROR
     */
    Error ImportRoadBook(std::string filepath, std::string bookname, bool overwrite, RoadBook *& roadbook);

    /**
     * Save the road book.
     * To be called if you created a new road book or if edited one.
     *
     * @param roadbook: the road book object.
     * @return @see ERROR
     */
    Error SaveRoadBook(RoadBook *& roadbook);

    /**
     * Release a given roadbook object. Must be called after Get or Create to release the given object when not needed anymore.
     * It does not save the file (@sa SaveRoadBook).
     *
     * @param roadbook: The object to release.
     * @return @see ERROR
     */
    Error ReleaseRoadBook(RoadBook *& roadbook);

    /**
     * Delete a book. Remove the file.
     *
     * @param bookname: the bookname
     * @return @see ERROR
     */
    Error DeleteRoadBook(std::string bookname);

    /**
     * Default constructor.
     */
    BookManager() {
    };

    /**
     * Destructor.
     */
    ~BookManager();

private:
    //! The root directory where books are stored (Full path).
    std::string RBookDirectory;

    //! The directory that roadbook can use as temporary archive.
    std::string RBookTempDirectory;

    //! The list of books.
    std::list<std::string> ListOfBooks;

};

} /* namespace RBook */
#endif /* BOOKMANAGER_H_ */
