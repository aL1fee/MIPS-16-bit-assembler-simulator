/*
 *
 */

#ifndef LOADER_H_
#define LOADER_H_

#include <fstream>
#include "control_unit.h"
#include "memory.h"
#include "Utility.h"

// The loader reads the object file and sets the memory (data and text portions).
// Its loadProgram(string filename) method returns a pointer to this memory which the control_unit recieves as an argument.
// The control unit does the actual processing.
class Loader {
private:
	Memory* memory;
	void init();

public:
	Loader();

	Memory* loadProgram(string filename);
};

#endif
