// tools.h

// By defining CMPT_ERROR_H, we avoid problems caused by including this file
// more than once: if CMPT_ERROR_H is already defined, then the code is *not*
// included.
#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cctype>

// C++ already has function called error, and so we put our error function
// inside a namespace called cmpt. Thus, to use this error function, we will
// write its full name, cmpt::error.
namespace cmpt {

// runtime_error is a pre-defined C++ object meant to be "thrown" when an
// error occurs while a program is running. When it is thrown, the program
// will end and print the given error message.
inline void error(const std::string& message)
{
    throw std::runtime_error(message);
}

// instead of having a function that lowers a string in every file
// put it here as a tool to call. 
inline void to_lower(std::string& message){
    std::transform(message.begin(), message.end(),
    message.begin(), ::tolower);
}

inline std::string trim(const std::string& str,
                 const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

} // namespace cmpt

#endif
