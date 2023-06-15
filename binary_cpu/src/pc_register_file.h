/*
 *
 */

#ifndef PC_REGISTER_FILE_H_
#define PC_REGISTER_FILE_H_

#include "register_file.h"

class PC_Register_file: public Register_file {
public:
	PC_Register_file(int n) : Register_file(n) {}

	void incrPC();
	Word getPC();
	void setPC(Word w);

	Word getNextPCWithoutIncr();

};

#endif /* PC_REGISTER_FILE_H_ */
