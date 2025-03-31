/*
* Author: Ol' Jim
* Date: 06/13/2012
* ByteForge Systems
* MIPS-Translatron 3000
*/

// dont get paid enough to implement

// Implemented by Evan

#include "../include/Instruction.h"

void ori_immd_assm(void) {
	if (strcmp(OP_CODE, "ORI") != 0) {
		// If the op code doesnt match, this isnt the correct command
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

	// Param 2 needs to be a register
	if (PARAM2.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// Param 3 needs to be an immediate
	if (PARAM3.type != IMMEDIATE) {
		state = INVALID_PARAM;
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

	// The immediate value is limited to 16 bits, this is 0xFFFF
	if (PARAM3.value > 0xFFFF) {
		state = INVALID_IMMED;
		return;
	}

	/*
		Putting the binary together
	*/

	// Set the opcode
	setBits_str(31, "001101");

	// Set Rt
	setBits_num(20, PARAM1.value, 5);

	// Set Rs
	setBits_num(25, PARAM2.value, 5);

	// Set the immediate value
	setBits_num(15, PARAM3.value, 16);

	// Tell the system the encoding is done
	state = COMPLETE_ENCODE;
}

void ori_immd_bin(void) {
	// Check if the opcode bits match
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches
	// any x will be skipped
	// If the manual shows (0), then the value of that bit doesn't matter
	if(checkBits(31, "001101") != 0) {
		// If the opcode doesn't match, this isn't the correct command
		state = WRONG_COMMAND;
		return;
	}

	// If the opcode bits match, check the type of parameters

	/*
		Finding values in the binary
	*/

	//getBits(start_bit, width)
	uint32_t Rs = getBits(25, 5);
	uint32_t Rt = getBits(20, 5);
	uint32_t immediate = getBits(15, 16);
	
	/*
		Setting instruction values
	*/

	setOp("ORI");
	//setParam(param_num, param_type, param_value)
	setParam(1, REGISTER, Rt); // destination register
	setParam(2, REGISTER, Rs); // source register
	setParam(3, IMMEDIATE, immediate); // immediate operand

	//tell the system the decoding is done
	state = COMPLETE_DECODE;

}



