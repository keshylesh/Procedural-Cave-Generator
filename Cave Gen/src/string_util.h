#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <sstream>
#include <fstream>

std::stringstream preprocessStream(std::istream&);
std::string getNextLine(std::istream&);
std::string trim(const std::string&);

std::stringstream preprocessStream(std::istream& istr) {
	istr.exceptions(istr.badbit | istr.failbit);
	std::stringstream ss;

	try {
		while (true) {
			std::string line = getNextLine(istr);
			ss << line << std::endl;
		}
	}
	catch (const std::exception& e) {
		if (!istr.eof()) {
			throw e;
		}
	}

	return ss;
}

std::string getNextLine(std::istream& istr) {
	const std::string comment = "#";
	std::string line = "";
	int i = 0;
	
	while (line == "") {
		if (istr.eof()) {
			return "EOF";
		}
		std::getline(istr, line);

		auto found = line.find(comment);
		if (found != std::string::npos) {
			line = line.substr(0, found);
		}
		line = trim(line);
	}
	return line;
}

std::string trim(const std::string& line) {
	const std::string whitespace = " \t\r\n";
	auto first = line.find_first_not_of(whitespace);
	if (first == std::string::npos) {
		return "";
	}
	auto last = line.find_last_not_of(whitespace);
	auto range = (last - first + 1);
	return line.substr(first, range);
}

#endif STRING_UTIL_H
