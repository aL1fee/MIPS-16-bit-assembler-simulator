/*
 *
 */

#include "control_unit.h"

Control_unit::Control_unit(Memory *mem) : generalRegFile(GENERAL_REGISTER_FILE_SIZE), HILORegFile(HILO_REGISTER_FILE_SIZE), PCRegFile(PC_REGISTER_FILE_SIZE) {
	alu = ALU();
	ram = mem;
}


// uncomment the following printing function comments to see each instruction and its address;
void Control_unit::run() {
	while (true) {
//		usleep(20000); // will sleep for .02s (for debugging)

		Word pc = PCRegFile.getPC();
		Word instr = ram->getValue(pc);
		if (instr.getBinaryValue() == EMPTY_INSTRUCTION_STRING) {
			break;
		} else {
//			cout << "PC: " << pc.getBinaryValue() << " ";
			execute_instruction(instr);
		}
	}
}

void Control_unit::execute_instruction(Word instr) {
//	cout << "INSTRUCTION: " << instr.getBinaryValue() << endl;
	string instrStr = instr.getBinaryValue();
	string opcode = instrStr.substr(OPCODE_START, OPCODE_SIZE);
	switch (Byte(opcode).convertToUDec()) {
		case INSTRUCTION_SYSCALL: {
			instruction_syscall(instrStr);
			break;
		}
		case INSTRUCTION_DIV: {
			instruction_div(instrStr);
			break;
		}
		case INSTRUCTION_MFLO: {
			instruction_mflo(instrStr);
			break;
		}
		case INSTRUCTION_MFHI: {
			instruction_mfhi(instrStr);
			break;
		}
		case INSTRUCTION_JR: {
			instruction_jr(instrStr);
			break;
		}
		case INSTRUCTION_JAL: {
			instruction_jal(instrStr);
			break;
		}
		case INSTRUCTION_BNE: {
			instruction_bne(instrStr);
			break;
		}
		case INSTRUCTION_BLE: {
			instruction_ble(instrStr);
			break;
		}
		case INSTRUCTION_J: {
			instruction_j(instrStr);
			break;
		}
		case INSTRUCTION_SUB: {
			instruction_sub(instrStr);
			break;
		}
		case INSTRUCTION_ADDI: {
			instruction_addi(instrStr);
			break;
		}
		case INSTRUCTION_ADD: {
			instruction_add(instrStr);
			break;
		}
		case INSTRUCTION_ORI: {
			instruction_ori(instrStr);
			break;
		}
		case INSTRUCTION_LUI: {
			instruction_lui(instrStr);
			break;
		}
		case INSTRUCTION_LW: {
			instruction_lw(instrStr);
			break;
		}
		case INSTRUCTION_SW: {
			instruction_sw(instrStr);
			break;
		}
	}
}

// IMPLEMENT more commands w/ ALU
void Control_unit::instruction_syscall(string s) { // syscall (SYSCALL_INSTRUCTION_STRING = 0000000000000001)
	Word v0Reg = generalRegFile.getElem(REGISTER_V0);
	int v0RegDec = v0Reg.convertToUDec();
	switch (v0RegDec) {
		case 1: { // print an integer in a0
			cout << generalRegFile.getElem(REGISTER_A0).convertToDec();
			PCRegFile.incrPC();
			break;
		}
		case 4: { // print a string the address of which is loaded into a0
			string result;
			Word strAddr = generalRegFile.getElem(REGISTER_A0);
			// use ALU for this
			while (true) {
				string strVal = ram->getValue(strAddr).getBinaryValue();
				string firstStrByte = strVal.substr(0, 8);
				string secondStrByte = strVal.substr(8, 8);
				if (firstStrByte == EMPTY_BYTE_STRING) {
					break;
				} else {
					result += char(Byte(firstStrByte).convertToUDec());
				}
				if (secondStrByte == EMPTY_BYTE_STRING) {
					break;
				} else {
					result += char(Byte(secondStrByte).convertToUDec());
				}
				strAddr = strAddr + Word(ADDRESS_INCREMENT);
			}
			result = Utility::replace_newline_chars_with_escape_char(result);
			cout << result;
			PCRegFile.incrPC();
			break;
		}
		case 5: {	// take integer input
			int input;
			cout << endl;
			cin >> input;

			Word inputValue = Word(Byte::decToSBin16(input));
			generalRegFile.setElem(inputValue, REGISTER_V0);
			PCRegFile.incrPC();

			break;
		}
		case 10: { // terminate the program
			exit(0);
		}
	}
}

void Control_unit::instruction_div (string s) {  //div Rs, Rt (opcode, rs, rt, 0000)
	int rs = Byte(s.substr(4, 4)).convertToUDec();
	int rt = Byte(s.substr(8, 4)).convertToUDec();
	if (isSettingZeroReg(rt) || isSettingSPReg(rs)) {
		return;
	}
	pair<Word, Word> result = alu.div(generalRegFile.getElem(rs) , generalRegFile.getElem(rt));
	HILORegFile.setElem(result.first, 0);
	HILORegFile.setElem(result.second, 1);
	PCRegFile.incrPC();
}

// quotient
void Control_unit::instruction_mflo (string s) { // mflo Rd
	int rd = Byte(s.substr(4, 4)).convertToUDec();
	if (isSettingZeroReg(rd) || isSettingSPReg(rd)) {
		return;
	}
	generalRegFile.setElem(HILORegFile.getElem(1), rd);
	PCRegFile.incrPC();
}

// remainder
void Control_unit::instruction_mfhi (string s) { // mfhi Rd
	int rd = Byte(s.substr(4, 4)).convertToUDec();
	if (isSettingZeroReg(rd) || isSettingSPReg(rd)) {
		return;
	}
	generalRegFile.setElem(HILORegFile.getElem(0), rd);
	PCRegFile.incrPC();
}

void Control_unit::instruction_jr (string s) { // jr Rs
	int rs = Byte(s.substr(4, 4)).convertToUDec();
	// IMPLEMENT change this to ALU operation!

	Word rsWord = generalRegFile.getElem(rs);
	PCRegFile.setPC(rsWord);
}

void Control_unit::instruction_jal (string s) { // jal Label
	string addrShortened =  s.substr(4, 12);
	string labelAddr = PCRegFile.getPC().getBinaryValue().substr(0, 3) + addrShortened + '0';
	Word resultAddr = Word(labelAddr);
	Word instrAfterJal = PCRegFile.getNextPCWithoutIncr();
	generalRegFile.setElem(instrAfterJal, REGISTER_RA);
	PCRegFile.setPC(resultAddr);
}

// branching only forwards! (unless you use jumps in branches to branch backwards)
// bne $s, $t,  target	(when you need to conditionally branch farther than 7 instructions ((2^4 -1) / 2), you
// should conditionally jump over a non-conditional jump that can reach the target:
// loop:	bne $t0, $a0, loop_cont
// 		j square_ret
// loop_cont:	add $s0, $s0, $a0
//		addi $t0, 1
// 		...
// square_ret ...
void Control_unit::instruction_bne (string s) { // bne Rs, Rt, Label
	int rs = Byte(s.substr(4, 4)).convertToUDec();
	int rt = Byte(s.substr(8, 4)).convertToUDec();
	int offset = Byte(s.substr(12, 4)).convertToUDec();
	// IMPLEMENT change this to ALU operation!
	if (alu.subtract(generalRegFile.getElem(rs), generalRegFile.getElem(rt)).getBinaryValue() != EMPTY_WORD_STRING) {
		PCRegFile.setPC(PCRegFile.getPC() + Word(offset));
	} else {
		PCRegFile.incrPC();
	}
}

//	 let $s0 = a, $s1 = b, $s2 = c, $v0 = return register
//   ble $s0, $s1, a_LTE_b                   # if(a <= b) branch(a_LTE_b)
//   ble $s0, $s2, max_C                     # if(a > b && a <=c) branch(max_C)
//   move $v0, $s0                           # else [a > b && a > c] max = a
//   j done
void Control_unit::instruction_ble (string s) { // ble Rs, Rt, Label
	int rs = Byte(s.substr(4, 4)).convertToUDec();
	int rt = Byte(s.substr(8, 4)).convertToUDec();
	int offset = Byte(s.substr(12, 4)).convertToUDec();
	// change this to ALU operation!

	Word difference = alu.subtract(generalRegFile.getElem(rs), generalRegFile.getElem(rt));

	if (difference.convertToDec() <= 0) {
		PCRegFile.setPC(PCRegFile.getPC() + Word(offset));
	} else {
		PCRegFile.incrPC();
	}
}

void Control_unit::instruction_j (string s) { // j Label
	string addrShortened =  s.substr(4, 12);
	string labelAddr = PCRegFile.getPC().getBinaryValue().substr(0, 3) + addrShortened + '0';
	Word resultAddr = Word(labelAddr);
//	cout << "J instr label addr: " << labelAddr << endl;
	PCRegFile.setPC(resultAddr);
}

void Control_unit::instruction_sub (string s) { // sub Rd, Rs, Rt
	int rd = Byte(s.substr(4, 4)).convertToUDec();
	int rs = Byte(s.substr(8, 4)).convertToUDec();
	int rt = Byte(s.substr(12, 4)).convertToUDec();
	if (isSettingZeroReg(rd) || isSettingSPReg(rd)) {
		return;
	}
	Word result = alu.subtract(generalRegFile.getElem(rs) , generalRegFile.getElem(rt));
	if (result.isCorrect()) {
		generalRegFile.setElem(result, rd);
		PCRegFile.incrPC();
	} else {
		cout << "Overflow occurs at the sub instruction at " << PCRegFile.getPC().getBinaryValue() << " address!" << endl;
		exit(0);
	}
}

void Control_unit::instruction_addi (string s) { // addiu Rd, Imm
	int rd = Byte(s.substr(4, 4)).convertToUDec();
	if (isSettingZeroReg(rd)) {
		return;
	}
	Byte immB = Byte(s.substr(IMM_START_INDEX, IMM_SIZE));
	if (!immB.isCorrect()) {
		cout << "Error at addi instruction at " << PCRegFile.getPC().getBinaryValue() << " address!" << endl;
		exit(0);
	}

	Word immW = Byte::decToSBin16(immB.convertToDec());

	Word result = alu.add(generalRegFile.getElem(rd), immW);
	if (result.isCorrect()) {
		if (rd == REGISTER_SP) {
			if (result.convertToDec() >= 0) {
				cout << "The stack space allocation is not correct at " << PCRegFile.getPC().getBinaryValue() << " address!" << endl;
				exit(0);
			}
		}
		generalRegFile.setElem(result, rd);
		PCRegFile.incrPC();
	} else {
		cout << "Error at addi instruction at " << PCRegFile.getPC().getBinaryValue() << " address!" << endl;
		exit(0);
	}
}

void Control_unit::instruction_add (string s) { // add Rd, Rs, Rt
	int rd = Byte(s.substr(4, 4)).convertToUDec();
	int rs = Byte(s.substr(8, 4)).convertToUDec();
	int rt = Byte(s.substr(12, 4)).convertToUDec();
	if (isSettingZeroReg(rd) || isSettingSPReg(rd)) {
		return;
	}
	Word result = alu.add(generalRegFile.getElem(rs) , generalRegFile.getElem(rt));
	if (result.isCorrect()) {
		generalRegFile.setElem(result, rd);
		PCRegFile.incrPC();
	} else {
		cout << "Overflow occurs at the add instruction at " << PCRegFile.getPC().getBinaryValue() << " address!" << endl;
		exit(0);
	}
}

void Control_unit::instruction_ori (string s) { // ori Rt, Imm
	int rt = Byte(s.substr(4, 4)).convertToUDec();
	if (isSettingZeroReg(rt) || isSettingSPReg(rt)) {
		return;
	}
	Word immW = Word(s.substr(IMM_START_INDEX, IMM_SIZE));
	Word result = alu.orOp(immW, generalRegFile.getElem(rt));
	if (result.isCorrect()) {
		generalRegFile.setElem(result, rt);
		PCRegFile.incrPC();
	} else {
		cout << "Error at ori instruction at " << PCRegFile.getPC().getBinaryValue() << " address!" << endl;
		exit(0);
	}
}

void Control_unit::instruction_lui (string s) { // lui Rt, Imm
	int rt = Byte(s.substr(4, 4)).convertToUDec();
	if (isSettingZeroReg(rt) || isSettingSPReg(rt)) {
		return;
	}
	Word immW = Word(s.substr(IMM_START_INDEX, IMM_SIZE));
	Word result = alu.lshift(immW, WORD_SIZE / 2);
	if (result.isCorrect()) {
		generalRegFile.setElem(result, rt);
		PCRegFile.incrPC();
	} else {
		cout << "Error at lui instruction at " << PCRegFile.getPC().getBinaryValue() << " address!" << endl;
		exit(0);
	}
}

void Control_unit::instruction_lw (string s) { // lw Rt, offset(Rs)
	int rt = Byte(s.substr(4, 4)).convertToUDec();
	int rs = Byte(s.substr(8, 4)).convertToUDec();
	int offset = Byte(s.substr(12, 4)).convertToUDec();
	if (isSettingZeroReg(rt) || isSettingSPReg(rt)) {
		return;
	}
	if (offset % 2 == 1) {
		cout << "Only even offset is allowed at " << PCRegFile.getPC().getBinaryValue() << " address!" << endl;
		exit(0);
	}
	Word offsetWord = Word(offset);
	Word rsWord = generalRegFile.getElem(rs);
	Word loadedW = ram->getValue(rsWord + offsetWord);
	if (loadedW.isCorrect()) {
		generalRegFile.setElem(loadedW, rt);
		PCRegFile.incrPC();
	} else {
		cout << "Error at lw instruction at " << PCRegFile.getPC().getBinaryValue() << " address!" << endl;
		exit(0);
	}
}

void Control_unit::instruction_sw (string s) { // sw Rt, offset(Rs)
	int rt = Byte(s.substr(4, 4)).convertToUDec();
	int rs = Byte(s.substr(8, 4)).convertToUDec();
	int offset = Byte(s.substr(12, 4)).convertToUDec();
	if (isSettingZeroReg(rt) || isSettingSPReg(rt)) {
		return;
	}
	if (offset % 2 == 1) {
		cout << "Only even offset is allowed at " << PCRegFile.getPC().getBinaryValue() << " address!" << endl;
		exit(0);
	}
	Word offsetWord = Word(offset);
	Word rsWord = generalRegFile.getElem(rs);
	Word swLocationWord = rsWord + offsetWord;
	if (rs == REGISTER_SP) {
		if (swLocationWord.convertToDec() >= 0) {
			cout << "The stack space allocation is not correct at " << PCRegFile.getPC().getBinaryValue() << " address!" << endl;
			exit(0);
		}

	}
	ram->insert(rsWord + offsetWord, generalRegFile.getElem(rt));
	PCRegFile.incrPC();

//	The store word instruction, sw, copies data from a register to memory. The register is not changed. The memory address is specified using a base/register pair.
// for sp, check that the value cannot go above 1111111111111111 -> can only subtract at that value.
}

bool Control_unit::isSettingZeroReg(int i) {
	if (i == REGISTER_ZERO) {
		cout << "Cannot assign to register $zero!" << endl;
		exit(0);
	}
	return false;
}

bool Control_unit::isSettingSPReg(int i) {
	if (i == REGISTER_SP) {
		cout << "Cannot assign to register $sp!" << endl;
		exit(0);
	}
	return false;
}

