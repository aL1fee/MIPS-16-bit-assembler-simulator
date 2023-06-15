/*
 *
 */

#ifndef REGISTER_FILE_H_
#define REGISTER_FILE_H_

#include "constants.h"
#include "number_representation.h"
#include <iostream>

using namespace std;

class Register_file {
protected:
	Word* reg_array;
	int size;

	void initialize();


public:
	Register_file(int n);

	void setElem(Word w, int i);
	Word getElem(int i);
	int getSize();

};

#endif /* REGISTER_FILE_H_ */
