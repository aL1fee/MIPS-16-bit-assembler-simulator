/*
 *
 */

#include <iostream>

#include "assembler.h"
#include "number_representation.h"
#include "memory.h"
#include "assembler.h"
#include "register_file.h"
#include "ALU.h"
#include "loader.h"
#include "control_unit.h"
#include "Utility.h"

using namespace std;


int main() {
	Assembler assembler = Assembler();
	string objectFilename = assembler.assemble(ASSEMBLY_PROGRAM_FILE_LOCATION);
	Loader loader = Loader();
	Memory* m = loader.loadProgram(objectFilename);
	Control_unit cpu = Control_unit(m);
	cpu.run();
}
