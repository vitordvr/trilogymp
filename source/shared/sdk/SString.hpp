#pragma once
#include "SString.h"
#include <cassert>
#include <algorithm>



//
// Format a string
//
SString& SString::vFormat(const char* szFormat, va_list vl)
{
    va_list vlLocal;

    // Guess size
    int iRequiredCapacity = 220;

    // Allocate buffer
    char* szDest = static_cast<char*>(malloc(iRequiredCapacity + 1));

    // Try to format the string into the buffer. If we will need
    // more capacity it will return -1 in glibc 2.0 and a greater capacity than
    // current in glibc 2.1, so we will resize. Else we've finished.
    va_copy(vlLocal, vl);
    int iSize = vsnprintf(szDest, iRequiredCapacity, szFormat, vlLocal);
    if (iSize == -1)
    {
        // glibc 2.0 - Returns -1 when it hasn't got enough capacity.
        // Duplicate the buffer size until we get enough capacity
        do
        {
            iRequiredCapacity *= 2;
            szDest = static_cast<char*>(realloc(szDest, iRequiredCapacity + 1));
            va_copy(vlLocal, vl);
            iSize = vsnprintf(szDest, iRequiredCapacity, szFormat, vlLocal);
        } while (iSize == -1);
    }
    else if (iSize > iRequiredCapacity)
    {
        // glibc 2.1 - Returns the required capacity.
        iRequiredCapacity = iSize + 1;
        szDest = static_cast<char*>(realloc(szDest, iRequiredCapacity + 1));

        va_copy(vlLocal, vl);
        iSize = vsnprintf(szDest, iRequiredCapacity, szFormat, vlLocal);
    }

    if (iSize < 1)
    {
        // Error or empty string
        clear();
    }
    else
    {
        // Copy from buffer
        szDest[iSize] = '\0';
        std::string::assign(szDest);
    }

    // Delete buffer
    free(szDest);

    // Done
    return *this;
}
