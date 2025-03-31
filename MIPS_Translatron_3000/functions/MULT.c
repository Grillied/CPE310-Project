#include "../include/Instruction.h"

// will do later

// Did later by Evan

void mult_reg_assm(void) {
	if(strcmp(OP_CODE, "MULT") != 0) {
		// If the opcode doesn't match, this isn't the correct command
		state = WRONG_COMMAND;
		return;
	}

	
	/*
		Checking the type of parameters
	*/
	// The first parameter should be a register
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// The second parameter should also be a register
	if (PARAM2.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	/*
		Checking the value of parameters
	*/
	// Rt should be 31 or less
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	// Rs should be 31 or less
	if (PARAM2.value > 31) {
		state = INVALID_REG;
		return;
	}

	/*
		Putting the binary together
	*/

	// Set the opcode
	setBits_str(31, "000000");

	// Set Rs
	setBits_num(25, PARAM1.value, 5); 

	// Set Rt
	setBits_num(20, PARAM2.value, 5);

	setBits_str(5, "011000");

	setBits_num(15, 0, 10); 

	// tell the system the encoding is done
	state = COMPLETE_ENCODE;
}

void mult_reg_bin(void) {
	// Check if the op code bits match
	if(checkBits(31, "000000") != 0 || checkBits(5, "011000") != 0) {
		// If the opcode doesn't match, this isn't the correct command
		state = WRONG_COMMAND;
		return;
	}

	// If the opcode bits match, check the type of parameters

	/*
		Finding values in the binary
	*/

	uint32_t Rs = getBits(25, 5);
	uint32_t Rt = getBits(20, 5);

	/*
		Setting instruction values
	*/

	setOp("MULT");

	setParam(1, REGISTER, Rs);
	setParam(2, REGISTER, Rt);

	state = COMPLETE_DECODE;
}

