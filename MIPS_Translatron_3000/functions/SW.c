/*
* Author: Ol' Jim
* Date: 06/13/2012
* ByteForge Systems
* MIPS-Translatron 3000
*/

/*
* William's Fixes:
* 1. Added clarifying comments
* 2. Fixed invalid immediate for second parameter in encoding
# 3. Corrected bits set for all parameters in encoding
* 3. Corrected how parameters were set in decoding
*/

#include "../include/Instruction.h"

void sw_immd_assm(void) {

	if (strcmp(OP_CODE, "SW") != 0) {
		// If the op code doesnt match, this isnt the correct command
		state = WRONG_COMMAND;
		return;
	}

	/*
		Checking the type of parameters
	*/

	// Param 1 should be a register
	if (PARAM1.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	// Param 2 should be an immediate
	if (PARAM2.type != IMMEDIATE) {
		state = INVALID_IMMED; // Fix to be invalid immediate instead of invalid register
		return;
	}	

	// Param 3 should be a register
	if (PARAM3.type != REGISTER) {
		state = MISSING_REG;
		return;
	}

	/*
		Checking the value of parameters
	*/

	// Destination register can be 0-31
	if (PARAM1.value > 31) {
		state = INVALID_REG;
		return;
	}

	// offset is limited to 16 bits, 0xF is 4 bits
	// offset comes before source register
	if (PARAM2.value > 0xFFFF) {
		state = INVALID_IMMED;
		return;
}

	// Source register can be 0-31
	if (PARAM3.value > 31) {
		state = INVALID_REG;
		return;
	}

	/*
		Putting the binary together
	*/
	
	// corrected setbit values 

	// Opcode set in first six bits from left
	setBits_str(31, "101011"); // Changed to correct opcode for SW

    // destination register set in 5 bits after source 
	setBits_num(20, PARAM1.value, 5); // Changed from 15 to 20

	// offset set in last 16 bits
	setBits_num(15, PARAM2.value, 16); // Changed from 25 to 15

	// source register set in 5 bits after opcode 
	setBits_num(25, PARAM3.value, 5); // Changed from 20 to 25

	// Tell the system the encoding is done
	state = COMPLETE_ENCODE;
}

void sw_immd_bin(void) {
	//check if bits, 31-26, match store word opcode
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches
	if (checkBits(31, "101011") != 0) {
		state = WRONG_COMMAND;
		//if not matching return to caller
		return;
	}

	/*
		Finding values in the binary
	*/

	//number for source register determined from bits 25-21
	uint32_t Rs = getBits(25, 5);
	//number for destination register determined from bits 20-16
	uint32_t Rt = getBits(20, 5);
	//offset value determined from bits 15-0
	uint32_t offset = getBits(15, 16);

	/*
		Setting Instuction values
	*/

	// set to store word operation
	setOp("SW");

	// setParam(param_num, param_type, param_value)
	// set destination register to determined register #
	setParam(1, REGISTER, Rt);

	// set offset to determined integer
	setParam(2, IMMEDIATE, offset); // Changed from register Rs to immediate

	// set source register 
	setParam(3, REGISTER, Rs); // Changed from immediate to register Rs

	// Tell the system the decoding is done
	state = COMPLETE_DECODE;
}