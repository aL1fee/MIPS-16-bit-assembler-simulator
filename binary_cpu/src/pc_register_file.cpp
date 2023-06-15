/*
 *
 */

#include "pc_register_file.h"

void PC_Register_file::incrPC() {
	reg_array[0] = reg_array[0] + Word(ADDRESS_INCREMENT);
}

Word PC_Register_file::getPC() {
	return reg_array[0];
}

Word PC_Register_file::getNextPCWithoutIncr() {
	return reg_array[0] + Word(ADDRESS_INCREMENT);
}

void PC_Register_file::setPC(Word w) {
	reg_array[0] = w;
}

