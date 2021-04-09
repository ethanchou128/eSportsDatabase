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

namespace helper {

	inline std::string ltrim(const std::string& s, int begin){
		std::string hold = s;
		if (begin < s.size() - 1){

			if (s.at(0) != ' '){
				hold = ltrim(s, s.size() + 1);
				return hold;
			}
			//checks the leading spaces and stops recursion when leading is found
			if (s.at(begin) == ' ' && s.at(begin + 1) != ' '){
				// s.substr(begin + 1) returns string without leading spaces
				hold = ltrim(s.substr(begin + 1), s.size() + 1); // s.size() + 1 breaks recursion
			} else {
				hold = ltrim(s, begin + 1); // Parses from left side to find all spaces
			}
		}
		return hold;
	}

	inline std::string rtrim(const std::string& s, int begin){
		std::string hold = s;
		if (begin > 0){
			if (s.at(s.size() - 1) != ' '){
				hold = rtrim(s, -1);
				return hold;
			}
			//checks the trailing spaces and stops recursion when leading is found
			if (s.at(begin) == ' ' && s.at(begin - 1) != ' '){
				// removes all trailing spaces and stops recursion
				hold = rtrim(hold.erase(begin), -1);
			} else {
				hold = rtrim(s, begin - 1);
			}
		}
		return hold;
	}

}

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
	inline std::string to_lower(std::string message){
		// std::transform(message.begin(), message.end(),
		// message.begin(), ::tolower);

		for (char &ch : message){
			ch = std::tolower(ch);
		}

		return message;
	}

	// inline std::string trim(const std::string& str,
	//                  const std::string& whitespace = " \t")
	// {
	//     const auto strBegin = str.find_first_not_of(whitespace);
	//     if (strBegin == std::string::npos)
	//         return ""; // no content

	//     const auto strEnd = str.find_last_not_of(whitespace);
	//     const auto strRange = strEnd - strBegin + 1;

	//     return str.substr(strBegin, strRange);
	// }


	inline std::string trim(const std::string& s){
		std::string hold = s;
		// handles empty string
		if (!s.empty()){
			hold = helper::ltrim(s, 0);
			hold = helper::rtrim(hold, hold.size() - 1);
		} else {
			hold = "";
		}

		return hold;
	}

	inline std::string clean(std::string& s){
		s = cmpt::to_lower(s);
		s = cmpt::trim(s);
		return s;
	}

	inline void sort_vecstr(std::vector<std::string>& vecstr){
		sort(vecstr.begin(), vecstr.end());
	}

} // namespace cmpt

#endif
