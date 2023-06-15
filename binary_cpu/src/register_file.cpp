/*
 *
 */

#include "register_file.h"


Register_file::Register_file(int n) {
	size = n;
	reg_array =  new Word[size];
	initialize();
}

void Register_file::initialize() {
	for (int i = 0; i < size; i++) {
		reg_array[i] = Word();
	}
	if (size == GENERAL_REGISTER_FILE_SIZE) {
		reg_array[REGISTER_SP] = Word(REGISTER_SP_STARTING_VALUE_STRING);
	}
}

void Register_file::setElem(Word w, int i) {
	reg_array[i] = w;
}

Word Register_file::getElem(int i) {
	return reg_array[i];
}

int Register_file::getSize() {
	return size;
}
