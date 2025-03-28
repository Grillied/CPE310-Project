/*
* Author: Ol' Jim
* Date: 06/13/2012 
* ByteForge Systems
* MIPS-Translatron 3000
*/

/*
* Cameron's Fixes:
* 1. Corrected parameter order in encoding (Rs and Rt were swapped)
* 2. Adjusted immediate value range check to if it is greater than 0xFFFF, not 0x7FFF
* 3. Fixed decoding to correctly assign Rs as base register and Rt as destination register
* 4. Improvded comments for better clarity
*/

#include "../include/Instruction.h"

void lw_immd_assm(void) {
	if (strcmp(OP_CODE, "LW") != 0) {
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

	// The second parameter should be an immediate
	if (PARAM2.type != IMMEDIATE) {
		state = INVALID_IMMED;
		return;
	}

	// The third parameter should be a register
	if (PARAM3.type != REGISTER) {
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
	if (PARAM3.value > 31) {
		state = INVALID_REG;
		return;
	}

	// The immediate value is limited to 16 bits, this is 0xFFFF 
	if (PARAM2.value > 0xFFFF) { // swapped 0x7FFF to 0xFFFF
		state = INVALID_IMMED;
		return;
	}

	/*
		Putting the binary together
	*/
	
	// Set the opcode
	setBits_str(31, "100011");

	// Set Rs
	setBits_num(25, PARAM3.value, 5);

	// Set Rt
	setBits_num(20, PARAM1.value, 5); 

	// Set immediate offset
	setBits_num(15, PARAM2.value, 16); 

	// Tell the system the encoding is done
	state = COMPLETE_ENCODE;
}

void lw_immd_bin(void) {
	// Check if the opcode bits match
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches
	// If the manual shows (0), then the value of that bit doesn't matter
	if (checkBits(31, "100011") != 0) {
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
	uint32_t imm16 = getBits(15, 16); // Cast immedite to int16_t to get sign extension

	/*
		Setting instruction values
	*/

	setOp("LW");

	// Corrected order so that Rs is first
	setParam(1, REGISTER, Rt);
	setParam(3, REGISTER, Rs);
	setParam(2, IMMEDIATE, imm16);

	// Tell the system the decoding is done
	state = COMPLETE_DECODE;
}
