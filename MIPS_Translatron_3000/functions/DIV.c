/*
* Author: Cameron Hannay
* Date: 03/26/2025 
* ByteForge Systems
* MIPS-Translatron 3000
*/

#include "../include/Instruction.h"

void div_reg_assm(void) {
	if (strcmp(OP_CODE, "DIV") != 0) {
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

	setBits_str(5, "011010");

	setBits_num(15, 0, 10);
	
	// Tell the system the encoding is done
	state = COMPLETE_ENCODE;
}

void div_reg_bin(void) {
	// Check if the opcode bits match
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches
	// If the manual shows (0), then the value of that bit doesn't matter
	if (checkBits(31, "000000") != 0 || checkBits(5, "011010") != 0) {
		state = WRONG_COMMAND;
		return;
	}

	// If the opcode bits match, then the rest can be read as correctly

	/*
		Finding values in the binary
	*/

	// getBits(start_bit, width)
	uint32_t Rs = getBits(25, 5);
	uint32_t Rt = getBits(20, 5);

	/*
		Setting instruction values
	*/
	
	setOp("DIV");

	// Corrected order so that Rs is first
	setParam(1, REGISTER, Rs); // dividend
	setParam(2, REGISTER, Rt); // divisor

	// Tell the system the decoding is done
	state = COMPLETE_DECODE;
}


