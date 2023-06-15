/*
 *
 */

#include "Utility.h"


string Utility::get_str_between_two_delims(const string &str, const char &start, const char &stop) {
	int sPos = str.find_first_of(start);
	int ePos = str.find_last_of(stop);
	return str.substr(sPos + 1, ePos - (sPos + 1));
}

string Utility::get_str_before_delim(const string &str, const char &delim) {
	return str.substr(0, str.find(delim));
}

string Utility::replace_newline_chars_with_escape_char(const string &str) {
	string result = "";
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == char(92) && (i + 1 < str.size()) && str[i+1] == char(110)) {
			result += '\n';
			i++;
		} else {
			result += str[i];
		}
	}
	return result;
}


map<int, string> Utility::splitIntoWordsUntilComments(string line) {
	map<int, string> lineWords;
	string str = "";
	int wordPos = 0;
	for (int i = 0; i < line.size(); i++) {
		// do comments
		if (line[i] == '#') {
			break;
		}
		if (line[i] == ',' || line[i] == ' ' || line[i] == '\t') {
			if (str != "") {
				lineWords[wordPos++] = str;
				str = "";
			} else {
				continue;
			}
		} else {
			str = str + line[i];
			if (i == line.size() - 1) {
				lineWords[wordPos] = str;
			}
		}
	}
	return lineWords;
}
