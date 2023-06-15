/*
 *
 */

#ifndef CONTROL_UNIT_H_
#define CONTROL_UNIT_H_

#include <utility>
#include <iostream>
#include <unistd.h>
#include "number_representation.h"
#include "memory.h"
#include "register_file.h"
#include "pc_register_file.h"
#include "Utility.h"
#include "ALU.h"

class Control_unit {
private:
	ALU alu;
	Register_file generalRegFile;
	Register_file HILORegFile;
	PC_Register_file PCRegFile;
	Memory* ram;

	void execute_instruction(Word instr);

	void instruction_syscall(string s);
	void instruction_div (string s);
	void instruction_mflo (string s);
	void instruction_mfhi (string s);
	void instruction_jr (string s);
	void instruction_jal (string s);
	void instruction_bne (string s);
	void instruction_ble (string s);
	void instruction_j (string s);
	void instruction_sub (string s);
	void instruction_addi (string s);
	void instruction_add (string s);
	void instruction_ori (string s);
	void instruction_lui (string s);
	void instruction_lw (string s);
	void instruction_sw (string s);

	bool isSettingZeroReg(int i);
	bool isSettingSPReg(int i);

public:
	Control_unit(Memory *mem);

	void run();
};

#endif /* CONTROL_UNIT_H_ */
