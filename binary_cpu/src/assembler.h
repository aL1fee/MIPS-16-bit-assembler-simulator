/*
 *
 */


#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <iostream>
#include <map>
#include <fstream>
#include <utility>
#include "Utility.h"
#include "number_representation.h"

using namespace std;

class Assembler {
private:
	map<string, int> commandMap;
	map<string, int> generalRegisterMap;

	int dataSize;
	int textSize;

	map<string, Word> symbolTable; // {label, address}

	void init();

	void firstPass(string filename);
	void secondPass();

	void printSymbolTable();


public:
	Assembler();

	string assemble(string filename); //return the filename of the object file

};

#endif /* ASSEMBLER_H_ */
