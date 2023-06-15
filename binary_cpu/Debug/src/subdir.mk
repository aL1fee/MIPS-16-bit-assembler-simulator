################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ALU.cpp \
../src/Utility.cpp \
../src/assembler.cpp \
../src/binary_cpu.cpp \
../src/control_unit.cpp \
../src/loader.cpp \
../src/memory.cpp \
../src/number_representation.cpp \
../src/pc_register_file.cpp \
../src/register_file.cpp 

CPP_DEPS += \
./src/ALU.d \
./src/Utility.d \
./src/assembler.d \
./src/binary_cpu.d \
./src/control_unit.d \
./src/loader.d \
./src/memory.d \
./src/number_representation.d \
./src/pc_register_file.d \
./src/register_file.d 

OBJS += \
./src/ALU.o \
./src/Utility.o \
./src/assembler.o \
./src/binary_cpu.o \
./src/control_unit.o \
./src/loader.o \
./src/memory.o \
./src/number_representation.o \
./src/pc_register_file.o \
./src/register_file.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/ALU.d ./src/ALU.o ./src/Utility.d ./src/Utility.o ./src/assembler.d ./src/assembler.o ./src/binary_cpu.d ./src/binary_cpu.o ./src/control_unit.d ./src/control_unit.o ./src/loader.d ./src/loader.o ./src/memory.d ./src/memory.o ./src/number_representation.d ./src/number_representation.o ./src/pc_register_file.d ./src/pc_register_file.o ./src/register_file.d ./src/register_file.o

.PHONY: clean-src

