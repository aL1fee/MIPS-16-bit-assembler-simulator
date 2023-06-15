/*
 *
 */

#include "assembler.h"

Assembler::Assembler() {
	init();
}

void Assembler::init() {
	commandMap["syscall"] = INSTRUCTION_SYSCALL;
	commandMap["div"] = INSTRUCTION_DIV;
	commandMap["mflo"] = INSTRUCTION_MFLO;
	commandMap["mfhi"] = INSTRUCTION_MFHI;
	commandMap["jr"] = INSTRUCTION_JR;
	commandMap["jal"] = INSTRUCTION_JAL;
	commandMap["bne"] = INSTRUCTION_BNE;
	commandMap["ble"] = INSTRUCTION_BLE;
	commandMap["j"] = INSTRUCTION_J;
	commandMap["sub"] = INSTRUCTION_SUB;
	commandMap["addi"] = INSTRUCTION_ADDI;
	commandMap["add"] = INSTRUCTION_ADD;
	commandMap["ori"] = INSTRUCTION_ORI;
	commandMap["lui"] = INSTRUCTION_LUI;
	commandMap["lw"] = INSTRUCTION_LW;
	commandMap["sw"] = INSTRUCTION_SW;
	commandMap["move"] = INSTRUCTION_MOVE;
	commandMap["li"] = INSTRUCTION_LI; // li: lui + ori
	commandMap["la"] = INSTRUCTION_LA; // lui + ori (for getting labels from memory -> la $1,label where $1=Address of label)

	generalRegisterMap["$zero"] = REGISTER_ZERO;
	generalRegisterMap["$v0"] = REGISTER_V0;
	generalRegisterMap["$v1"] = REGISTER_V1;
	generalRegisterMap["$a0"] = REGISTER_A0;
	generalRegisterMap["$a1"] = REGISTER_A1;
	generalRegisterMap["$t0"] = REGISTER_T0;
	generalRegisterMap["$t1"] = REGISTER_T1;
	generalRegisterMap["$t2"] = REGISTER_T2;
	generalRegisterMap["$t3"] = REGISTER_T3;
	generalRegisterMap["$s0"] = REGISTER_S0;
	generalRegisterMap["$s1"] = REGISTER_S1;
	generalRegisterMap["$s2"] = REGISTER_S2;
	generalRegisterMap["$s3"] = REGISTER_S3;
	generalRegisterMap["$s4"] = REGISTER_S4;
	generalRegisterMap["$sp"] = REGISTER_SP;
	generalRegisterMap["$ra"] = REGISTER_RA;

	dataSize = 0;
	textSize = 0;
}

// Printing symbol table for debugging
void Assembler::printSymbolTable() {
	for(map<string, Word>::iterator it = symbolTable.begin(); it != symbolTable.end(); ++it) {
		cout << "Key: " << it->first << "; Value: " << it->second.getBinaryValue() << endl;
	}
}

void Assembler::firstPass(string filename) {
	fstream file_in;
	ofstream file_out;

	file_in.open(filename, ios::in);
	file_out.open(ASSEMBLY_FIRST_PASS_OUTPUT_FILE_LOCATION, ios::out);

	if (file_in.is_open()) {
		string line;
		bool atDataSegment = false;
		Word currentAddress = Word();

		while (getline(file_in, line)) {
			map<int, string> lineWords = Utility::splitIntoWordsUntilComments(line);

			if (lineWords[0] == ".data") {
				file_out << "Data segment\n";
				atDataSegment = true;
				currentAddress = Word(DATA_INITIAL_ADDRESS);
				continue;
			}
			if (lineWords[0] == ".text") {
				file_out << "Text segment\n";
				atDataSegment = false;
				currentAddress = Word(TEXT_INITIAL_ADDRESS);
				continue;
			}



			if (atDataSegment) {
					string key;
				int value = 0;
				if (lineWords[0][lineWords[0].size() - 1] == ':') {
					key = lineWords[0].substr(0, lineWords[0].size() - 1);
					//
					symbolTable[key] = Word(currentAddress.getBinaryValue());
					//
				}
				if (lineWords[1] == ".word") {
					for (int i = 2; i < lineWords.size(); i++) {
						value = stoi(lineWords[i]);
						file_out << currentAddress.getBinaryValue() << " " << Byte::decToSBin16(value) << endl;
						currentAddress = currentAddress + Word(ADDRESS_INCREMENT);

					}
				} else if (lineWords[1] == ".asciiz") {
					string str = Utility::get_str_between_two_delims(line, '\"', '\"');
					int index = 0;
					while (index < str.size()) {
						string result = EMPTY_WORD_STRING;
						string charStr = Byte::decToUBin8(int(str[index++]));

						if (index < str.size()) {
							result = charStr + Byte::decToUBin8(int(str[index++]));
						} else {
							result = charStr + EMPTY_BYTE_STRING;
						}

						file_out << currentAddress.getBinaryValue() << " " << result << endl;
						currentAddress = currentAddress + Word(ADDRESS_INCREMENT);

						if (index >= str.size()) {
							result = EMPTY_WORD_STRING;
							file_out << currentAddress.getBinaryValue() << " " << EMPTY_WORD_STRING << endl;
							currentAddress = currentAddress + Word(ADDRESS_INCREMENT);
						}
					}

				}
			} else {
				string key;
				if (lineWords[0][lineWords[0].size() - 1] == ':') {
					key = lineWords[0].substr(0, lineWords[0].size() - 1);
					symbolTable[key] = currentAddress;
					for (int i = 1; i < lineWords.size(); i++) {
						lineWords[i - 1] = lineWords[i];
					}
					lineWords.erase(lineWords.size() - 1);
				}
				string opcode, instr = "";
				try {
					// handling pseudoinstructions
					if (commandMap.at(lineWords[0]) > 15) {
						switch (commandMap.at(lineWords[0])) {
							case INSTRUCTION_MOVE: { // move $s1, $s2 = add $s1, $s2, $zero
								opcode = Byte::decToUBin4(INSTRUCTION_ADD);
								instr = opcode + Byte::decToUBin4(generalRegisterMap.at(lineWords[1])) + Byte::decToUBin4(generalRegisterMap.at(lineWords[2])) +
										Byte::decToUBin4(generalRegisterMap.at("$zero"));
								file_out << currentAddress.getBinaryValue() << " " << instr << endl;
								currentAddress = currentAddress + Word(ADDRESS_INCREMENT);
								break;
							}
							case INSTRUCTION_LI: { //[li Rd, value]  ex. li $t0 -24023   ex. li $t1 0xab ===> lui $t1, 0xa  &  ori $t1, 0xb
								string imm16S = Byte::decToSBin16(stoi(lineWords[2]));
								opcode = Byte::decToUBin4(commandMap.at("lui"));
								instr = opcode + Byte::decToUBin4(generalRegisterMap.at(lineWords[1])) + imm16S.substr(0, 8);
								file_out << currentAddress.getBinaryValue() << " " << instr << endl;
								currentAddress = currentAddress + Word(ADDRESS_INCREMENT);
								opcode = Byte::decToUBin4(commandMap.at("ori"));
								instr = opcode + Byte::decToUBin4(generalRegisterMap.at(lineWords[1])) + imm16S.substr(8, 8);
								file_out << currentAddress.getBinaryValue() << " " << instr << endl;
								currentAddress = currentAddress + Word(ADDRESS_INCREMENT);
								break;
							}
							case INSTRUCTION_LA: { //[la Rd, Label]
								instr = "lui " + lineWords[1] + ", " + "%hi(" + lineWords[2] + ")";
								file_out << currentAddress.getBinaryValue() << " " << instr << endl;
								currentAddress = currentAddress + Word(ADDRESS_INCREMENT);
								instr = "ori " + lineWords[1] + ", " + "%lo(" + lineWords[2] + ")";
								file_out << currentAddress.getBinaryValue() << " " << instr << endl;
								currentAddress = currentAddress + Word(ADDRESS_INCREMENT);
								break;
							}
						}
					} else {
						opcode = Byte::decToUBin4(commandMap.at(lineWords[0]));
						switch (commandMap.at(lineWords[0])) {
							case INSTRUCTION_SYSCALL: // syscall
								instr = SYSCALL_INSTRUCTION_STRING;
								break;
							// 4 arguments (keyword + 3 regs)
							case INSTRUCTION_ADD:	// add Rd, Rs, Rt
							case INSTRUCTION_SUB:	// sub Rd, Rs, Rt
								instr = opcode + Byte::decToUBin4(generalRegisterMap.at(lineWords[1])) + Byte::decToUBin4(generalRegisterMap.at(lineWords[2])) +
										Byte::decToUBin4(generalRegisterMap.at(lineWords[3]));
								break;
								// 4 arguments (keyword + 2 regs + label)
							case INSTRUCTION_BNE:	// bne Rs, Rt, Label
							case INSTRUCTION_BLE:	// beq Rs, Rt, Label
								instr = lineWords[0] + " " + lineWords[1] + ", " + lineWords[2] + ", " + lineWords[3];
								break;
							// 3 arguments
							case INSTRUCTION_DIV:	// mult Rs, Rt (opcode, rs, rt, 0000)
								instr = opcode + Byte::decToUBin4(generalRegisterMap.at(lineWords[1])) + Byte::decToUBin4(generalRegisterMap.at(lineWords[2])) + EMPTY_HALF_BYTE_STRING;
								break;
							// 3 arguemnts addressing (only positive offsets for now)
							case INSTRUCTION_LW:	// lw Rt, offset(Rs)
							case INSTRUCTION_SW: {	// sw Rt, offset(Rs)
								string offsetReg = Utility::get_str_between_two_delims(lineWords[2], '(', ')');
								string offset = Utility::get_str_before_delim(lineWords[2], '(');
								if (offset == "") {
									offset = "0";
								}
								instr = opcode + Byte::decToUBin4(generalRegisterMap.at(lineWords[1])) + Byte::decToUBin4(generalRegisterMap.at(offsetReg)) +
										Byte::decToUBin4(stoi(offset));
								break;
							}
							// 3 arguemnts w/ immediate
							case INSTRUCTION_ORI:	// lui Rt, Imm
							case INSTRUCTION_LUI: 	// ori Rt, Imm
								instr = opcode + Byte::decToUBin4(generalRegisterMap.at(lineWords[1])) + Byte::decToUBin8(stoi(lineWords[2]));
								break;
							case INSTRUCTION_ADDI:	// addiu Rd, Imm
								instr = opcode + Byte::decToUBin4(generalRegisterMap.at(lineWords[1])) + Byte::decToSBin8(stoi(lineWords[2]));
								break;
							// 2 arguments (keyword + reg)
							case INSTRUCTION_MFLO:	// mflo Rd
							case INSTRUCTION_MFHI:	// mfhi Rd
							case INSTRUCTION_JR:	// jr Rs
								instr = opcode + Byte::decToUBin4(generalRegisterMap.at(lineWords[1])) + EMPTY_BYTE_STRING;
								break;
							// 2 arguments (keyword + label)
							case INSTRUCTION_JAL:	// jal Label
							case INSTRUCTION_J:		// j Label
								instr = lineWords[0] + " " + lineWords[1];
								break;
						}
						file_out << currentAddress.getBinaryValue() << " " << instr << endl;
						currentAddress = currentAddress + Word(ADDRESS_INCREMENT);
					}
				} catch (const std::out_of_range& e) {
//					cout << "##### Incorrect command! ##### " << endl;
				}
			}
		}
		file_in.close();
		file_out.close();
	} else {
		cout << "The file could not be opened!" << endl;
	}
// uncomment is you want the symbol table to be printed;
//	printSymbolTable();
}

void Assembler::secondPass() {
	fstream file_in;
	ofstream file_out;

	file_in.open(ASSEMBLY_FIRST_PASS_OUTPUT_FILE_LOCATION, ios::in);
	file_out.open(ASSEMBLY_SECOND_PASS_OUTPUT_FILE_LOCATION, ios::out);

	if (file_in.is_open()) {
		string line;
		while (getline(file_in, line)) {
			map<int, string> lineWords = Utility::splitIntoWordsUntilComments(line);
			if (lineWords.size() <= 2) {
				file_out << line << endl;
				continue;
			}
			string opcode, instr = "";
			try {
				opcode = Byte::decToUBin4(commandMap.at(lineWords[1]));
				switch (commandMap.at(lineWords[1])) {
					case INSTRUCTION_LUI:
					case INSTRUCTION_ORI: {
						string imm = symbolTable[Utility::get_str_between_two_delims(lineWords[3], '(', ')')].getBinaryValue();
						string hi_lo = Utility::get_str_between_two_delims(lineWords[3], '%', '(');
						if (hi_lo == "hi") {
							imm = imm.substr(0, 8);
						} else if (hi_lo == "lo") {
							imm = imm.substr(8, 8);
						}
						instr = opcode + Byte::decToUBin4(generalRegisterMap.at(lineWords[2])) + imm;
						break;
					}
					case INSTRUCTION_BNE:
					case INSTRUCTION_BLE: {
						string branchInstrAddr = symbolTable[lineWords[4]].getBinaryValue();
						int instrDecDiff = Word(branchInstrAddr).convertToUDec() - Word(lineWords[0]).convertToUDec();
						if (instrDecDiff >= 16) {
							 cout << "##### Branch destination must be within 7 instructions! #####" << endl;
							 instr = INCORRECT_WORD_STRING;
							 break;
						}
						instr = opcode + Byte::decToUBin4(generalRegisterMap.at(lineWords[2])) + Byte::decToUBin4(generalRegisterMap.at(lineWords[3])) +
								Byte::decToUBin4(instrDecDiff);
						break;
					}
					case INSTRUCTION_J:
					case INSTRUCTION_JAL: {
						string destAddr = symbolTable[lineWords[2]].getBinaryValue();
						string shortenedAddr = destAddr.substr(3, 12);
						instr = opcode + shortenedAddr;
						break;
					}
				}
				file_out << lineWords[0] << " " << instr << endl;
			} catch (const std::out_of_range& e) {
//				cout << "##### Incorrect command! ##### " << endl;
			}
		}
	file_in.close();
	file_out.close();
	} else {
		cout << "The file could not be opened!" << endl;
	}
}

string Assembler::assemble(string filename) {
	firstPass(filename);
	secondPass();
	return ASSEMBLY_SECOND_PASS_OUTPUT_FILE_LOCATION;
}
