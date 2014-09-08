/*
 * Error.cpp
 *
 *  Created on: Aug 26, 2014
 *      Author: Luc Fourestier
 */

#include "Error.h"

namespace RBook {

Error::Error() :
        Code(ERROR_OK) {
}

Error::Error(int code) :
        Code(code) {
}

Error::Error(const Error& error) :
        Code(error.Code) {
    Description = error.Description;
}

Error& Error::operator=(const Error& error) {
    Code = error.Code;
    Description = error.Description;
    return *this;
}

Error& Error::operator=(const int& error) {
    Code = error;
    return *this;
}

Error::~Error() {
}

} // namespace

