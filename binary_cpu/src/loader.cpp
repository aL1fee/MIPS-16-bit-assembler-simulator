/*
 *
 */

#include "loader.h"

void Loader::init() {
	memory = new Memory();
}

Loader::Loader() {
	init();
}

Memory* Loader::loadProgram(string filename) {
	fstream file;
	file.open(filename, ios::in);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			map<int, string> lineWords = Utility::splitIntoWordsUntilComments(line);
			if (lineWords[0].size() != WORD_SIZE) {
				continue;
			}
			if (lineWords[0][0] == '0') {
				memory->incrTextSize();
			}
			memory->insert(Word(lineWords[0]), Word(lineWords[1]));
		}

		file.close();
	} else {
		cout << "Loader: The file could not be opened!" << endl;
	}
	return memory;
}
