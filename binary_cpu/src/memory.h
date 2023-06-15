/*
 *
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "number_representation.h"

class Memory {
private:
	Word* array;
	int size;
	int textIndex, dataIndex, stackIndex;
	int textSize;

	void init();
    int addressToIndex(string str);

public:
    Memory();

    void insert(Word address, Word instr);

    Word getValue(int i);
    Word getValue(Word w);


    int getTextSize();
    void incrTextSize();

    int getTextIndex();
    int getDataIndex();
    int getStackIndex();
	int getSize();
};

#endif /* MEMORY_H_ */


