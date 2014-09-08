/*
 * Error.h
 *
 *  Created on: Aug 26, 2014
 *      Author: Luc Fourestier
 */

#ifndef ERROR_H_
#define ERROR_H_

#include <string>

//! Set an std::string message to the error object.
#define ERROR_SET_MSG(error, string) error.Description.assign(string)

//! Get the error code as integer
#define ERROR_CODE(error) error.Code

namespace RBook {

//! Success
const int ERROR_OK = 0;
//! Generic failure
const int ERROR_FAIL = -1;

class Error {
public:
    /**
     * Default constructor.
     */
    Error();

    /**
     * Constructor with code
     *
     * @param Code: Error code
     */
    Error(int Code);

    /**
     * Copy constructor.
     *
     * @param error: Error object to copy.
     */
    Error(const Error& error);

    /**
     * Copy operator.
     */
    Error& operator=(const Error& error);

    /**
     * Copy from integer.
     */
    Error& operator=(const int& code);

    /**
     * Destructor
     */
    ~Error();

    //! Error code
    int Code;

    //! Error description
    std::string Description;


};

/**
 * Check error codes.
 */
inline bool operator==(const Error& lhs, const Error& rhs) {
    return (lhs.Code == rhs.Code);
}

/**
 * Check error codes.
 */
inline bool operator==(const Error& lhs, const int& rhs) {
    return (lhs.Code == rhs);
}

/**
 * Check error codes.
 */
inline bool operator==(const int& lhs, const Error& rhs) {
    return (lhs == rhs.Code);
}

/**
 * Check error codes.
 */
inline bool operator!=(const Error& lhs, const Error& rhs) {
    return !(lhs.Code == rhs.Code);
}

/**
 * Check error codes.
 */
inline bool operator!=(const Error& lhs, const int& rhs) {
    return !(lhs.Code == rhs);
}

/**
 * Check error codes.
 */
inline bool operator!=(const int& lhs, const Error& rhs) {
    return !(lhs == rhs.Code);
}

} // namespace

#endif /* ERROR_H_ */
