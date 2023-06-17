# MIPS 16 bit assembler simulator

A 16 bit assembler based on MIPS architecture.
The assembler reads an assembly language file (specified in constants.h ASSEMBLY_PROGRAM_FILE_LOCATION) and parses it in two passes. In the first pass, it splits pseudo-instructions into regular instructions, converts regular instructions that do not use labels into the machine code, assigns the exact address to each instruction, and creates a symbol table with labels and their respective addresses. The result is saved into doc/assembly_1.obj. In the second pass, the assembler converts all instructions that use labels into the machine code using the addresses for the required labels from the symbol table. The result is saved into doc/assembly_2.obj. 
After that, the loader reads doc/assembly_2.obj and sets the memory accordingly (data and text portions). Loader’s loadProgram(string filename) method returns a pointer to the memory object which the control unit receives as an argument. The control unit reads the machine code and does the program execution using ALU and other helper classes.

The assembler supports the following data types: integers, strings, lists of integers. It can output integers, strings, lists of integers. It can take integers as an input. You can find a number of examples of assembly programs in the doc/ directory. Among them are the following algorithms and procedures: a procedure for multiplication using addition, an algorithm to find the sum of the first n whole numbers, a procedure for finding a fibonacci number using iteration, a procedure for finding a fibonacci number using recursion, a procedure for printing an array of integers, an algorithm for reversing an array of integers, an algorithm for sorting an array using insertion sort.


Words are 16-bit or 2-byte long.

The assembler supports the following 16 regular and 3 pseudo instructions: 
- syscall 
- div Rs, Rt (divide)
- mflo Rd (move from low register)
- mfhi Rd (move from high register)
- j Label (jump)
- jr Rs (jump register)
- jal Label (jump and link)
- bne Rs, Rt, Label (branch if not equal)
- ble Rs, Rt, Label (branch if less than or equal)
- sub Rd, Rs, Rt (subtract) 
- addi Rd, Imm (add immediate from -127 to 128) 
- add Rd, Rs, Rt (add)
- ori Rt, Imm (or immediate)
- lui Rt, Imm (load upper immediate)
- lw Rt, offset(Rs) (load word)
- sw Rt, offset(Rs) (save word)
- move Rd, Rs => add $Rd, $Rs, $zero (move, pseudo-instruction)
- li Rd, Imm => lui Rd, Imm[0:7] & ori Rd, Imm[8:15] (load immediate from -32768 to 32767, pseudo-instruction)
- la Rd, Label => lui Rd, %hi(Label) & ori Rd, %lo(Label) (load address, pseudo-instruction)

Instruction formats: 
- 4-bit opcode, 4-bit register,  4-bit register,  4-bit register (add, sub)
- 4-bit opcode,  4-bit register, 4-bit register (div)
- 4-bit opcode, 12-bit Label (j, jal)
- 4-bit opcode, 4-bit register (jr, mflo, mfhi)
- 4-bit opcode, 4-bit register, 4-bit register, 4-bit Label offset (bne, ble; Label offset = Label address - current address. Label offset cannot be greater than 2^4 - 1. You can only branch forwards. If you need to branch backwards or to conditionally branch farther than 7 instructions ((2^4 -1) / 2), you should conditionally jump over a non-conditional jump that can reach the target:
    - loop:              bne $t0, $t1, loop_cont
    - 		      j square_ret
    - loop_cont:    add $s0, $s0, $a0
    - 		      addi $t0, 1
    - 		      ...
    - square_ret:  ...
- 4-bit opcode, 4-bit register, 4-bit register, 4-bit offset (lw, sw; offset cannot be greater than 2^4 - 1)
- 4-bit opcode, 4-bit register Rd, 8-bit immediate (addi; the immediate range is from -2^7 to 2^7 - 1)
- 4-bit opcode, 4-bit register Rt, 8-bit immediate (lui, ori)
- 4-bit opcode (syscall)


The simulated CPU has the following general purpose registers:
- $zero (always equal to 0)
- $v0 (return value 1)
- $v1 (return value 2)
- $a0 (argument value 1)
- $a1 (argument value 2)
- $s0
- $s1
- $s2
- $s3
- $s4
- $t0
- $t1
- $t2
- $t3
- $sp (stack pointer)
- $ra (return address of the last caller)

Registers $s0-$s4 are used for values that are expected to be preserved across any procedure calls
Registers $t0-$t3 are used for values that are not expected to be preserved (must be saved by the caller if need to be preserved)

The CPU has the following extra registers:
- HI (stores the remainder of division)
- LO (stores the quotient of division)
- PC (program counter, always aligned on 2-byte boundary)


Memory (RAM) size is 2^12 words (can be expanded):
- data (data objects; readable/writeable, starts at 1000000000000000, computed in memory as  0000100000000000)
- text (contains only instructions; read-only, starts at 0000000000000000)
- stack (grows down from that address; readable/writeable, starts at 111111111111111, computed in memory as 0000111111111111)

Memory addresses can be given by:
- symbolic name (label): lw $t0, Label
- indirectly via a register: lw $t1, ($t0)
- indexed addressing (the offset is always even and less than 2^4 - 1 ): lw $t1, 8($t0)

The address of the top element of the stack is stored in the $sp register. Need to move $sp down to make room for new data when storing it in the stack.



Below is an example of a program that is executed by the assembler. It calls a procedure that recursively computes a fibonacci number. 

The pseduocode:

<img width="266" alt="c_code_fib_recurs" src="https://github.com/aL1fee/MIPS-16-bit-assembler-simulator/assets/47246379/f875b25f-4ba4-4469-8b55-055de3dd3d5b">

The assembly program:

<img width="758" alt="fib_1" src="https://github.com/aL1fee/MIPS-16-bit-assembler-simulator/assets/47246379/c2785d34-1aed-4b22-aa53-ec42062678ed">
<img width="789" alt="fib_2" src="https://github.com/aL1fee/MIPS-16-bit-assembler-simulator/assets/47246379/ae36dd1b-6ff8-4af9-9ca7-5cfb129e57b4">

First assembler pass output:

<img width="178" alt="obj_1" src="https://github.com/aL1fee/MIPS-16-bit-assembler-simulator/assets/47246379/87039412-c91f-4362-b35a-ded513b5599e">

Second assembler pass output:

<img width="145" alt="obj_2" src="https://github.com/aL1fee/MIPS-16-bit-assembler-simulator/assets/47246379/285294bf-b4c5-47f6-922f-8563cf37625a">

The output:

<img width="220" alt="fib_output" src="https://github.com/aL1fee/MIPS-16-bit-assembler-simulator/assets/47246379/ea3c5820-278c-483d-aedc-33c1cb78c8a3">

