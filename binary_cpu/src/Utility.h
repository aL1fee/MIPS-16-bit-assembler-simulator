/*
 *
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <iostream>
#include <string>
#include <map>

using namespace std;

class Utility {
private:

public:
	Utility();

	static string get_str_between_two_delims(const string &str, const char &start, const char &stop);
	static string get_str_before_delim(const string &str, const char &delim);

	static string replace_newline_chars_with_escape_char(const string &str);

	static map<int, string> splitIntoWordsUntilComments(string line);

};

#endif /* UTILITY_H_ */
