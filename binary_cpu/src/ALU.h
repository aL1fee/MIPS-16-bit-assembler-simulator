/*
 *
 */

#ifndef ALU_H_
#define ALU_H_


#include <iostream>
#include <utility>
#include "constants.h"
#include "number_representation.h"

using namespace std;

class ALU {
private:


public:
	ALU() {};

	Word rshift(Word w, int n);
	Word lshift(Word w, int n);
	Word invert(Word w);

	Word andOp(Word w);
	Word orOp(Word w1, Word w2);
	Word xorOp(Word w);

	Word add(Word w1, Word w2);
	Word subtract(Word w1, Word w2);
	Word mul(Word w1, Word w2);
	pair<Word, Word> div(Word w1, Word w2);
};

#endif /* ALU_H_ */
