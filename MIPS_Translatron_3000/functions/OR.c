/*
* William's Fixes:
* 1. Added several more clarifying comments
*/

#include "../include/Instruction.h"

void or_reg_assm(void) {
	if (strcmp(OP_CODE, "OR") != 0) {
		// If the op code doesnt match, this isnt the correct command
		state = WRONG_COMMAND;
		return;
	}

	/*
		Checking the type of parameters
	*/

	//Param 1 should be a register
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	//Param 2 should be a register
	if (PARAM2.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	//Param 3 should be a register
	if (PARAM3.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	/*
		Checking the value of parameters
	*/

	//Destination register can be 0-31
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	//first source register can be 0-31
	if (PARAM2.value > 31) {
		state = INVALID_REG;
		return;
	}

	//second source register can be 0-31
	if (PARAM3.value > 31) {
		state = INVALID_REG;
		return;
	}

	/*
		Putting the binary together
	*/

	// Opcode set in first six bits from left
	setBits_num(31, 0, 6);

	// function code set in last six bits 
	setBits_str(5, "100101");

	// destination register set in 5 bits after second source
	setBits_num(15, PARAM1.value, 5);

	// first source register set in 5 bits after opcode
	setBits_num(25, PARAM2.value, 5);

	// second source register set in 5 bits after first source
	setBits_num(20, PARAM3.value, 5);

	// Tell the system the encoding is done
	state = COMPLETE_ENCODE;
}

void or_reg_bin(void) {
	//check if bits, 31-26, match register opcode
	//check if bits, 5-0, match OR function
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches
	if (checkBits(31, "000000") != 0 || checkBits(5, "100101") != 0) {
		state = WRONG_COMMAND;
		//if not matching return to caller
		return;
	}

	/*
		Finding values in the binary
	*/

	//number for destination register determined from bits 15-11
	uint32_t Rd = getBits(15, 5);
	//number for first source register determined from bits 25-21
	uint32_t Rs = getBits(25, 5);
	//number for second source register determined from bits 20-16
	uint32_t Rt = getBits(20, 5);
	
	/*
		Setting Instruction values
	*/

	//set to load OR function
	setOp("OR");

	//set destination register to determined register operand
	//setParam(param_num, param_type, param_value)
	setParam(1, REGISTER, Rd); 
	//set first source register operand
	setParam(2, REGISTER, Rs); 
	//set second source register operand
	setParam(3, REGISTER, Rt); 

	// Tell the system the decoding is done
	state = COMPLETE_DECODE;
}


