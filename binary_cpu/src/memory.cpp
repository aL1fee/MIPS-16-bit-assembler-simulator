/*
 *
 */

#include "memory.h"

Memory::Memory() {
	init();
}

void Memory::init() {
	size = DEFAULT_MEMORY_SIZE;
	array = new Word[size];
	textIndex = TEXT_INITIAL_INDEX;
	dataIndex = DATA_INITIAL_INDEX;
	stackIndex = STACK_INITIAL_INDEX;
	textSize = 0;
}

// 2^12 word memory is hardcoded; will have to change this and some other methods to change the memory size (MAX = 2^16 words = 2^17 bytes)
// 0000000000000000 -> 0
// 0000000000000010 -> 1
// 0000000000000100 -> 2
// 0000000000000110 -> 3
// 0000000000001000 -> 4
// 1000000000000000 -> 2048
// 1000000000000010 -> 2049
// 1000000000000100 -> 2050
// 1000000000000110 -> 2051
// 1000000000001000 -> 2052
int Memory::addressToIndex(string str) {
	string strCut;
	if (str[0] == '0') {
		strCut = "00000" + str.substr(4, 11);
	} else {
		strCut = "00001" + str.substr(4, 11);
	}
	return Word(strCut).convertToUDec();
}

void Memory::insert(Word address, Word instr) {
	string addr = address.getBinaryValue();
	array[addressToIndex(addr)] = instr;
}

Word Memory::getValue(Word w) {
	return array[addressToIndex(w.getBinaryValue())];
}

Word Memory::getValue(int i) {
	return array[i];
}

int Memory::getTextSize() {
	return textSize;
}

void Memory::incrTextSize() {
	textSize++;
}

int Memory::getTextIndex() {
	return textIndex;
}

int Memory::getDataIndex() {
	return dataIndex;
}

int Memory::getStackIndex() {
	return stackIndex;
}

int Memory::getSize() {
	return size;
}

