/*
* Author: Ol' Jim
* Date: 06/13/2012
* ByteForge Systems
* MIPS-Translatron 3000
*/

/*
* William's Fixes:
* 1. Added clarifying comments
* 2. Fixed parameter checks--deleted check for source register
* 3. Fixed bit assignments -- imaginary source register always
     set to "00000"
* 4. In dissassembler, added a check to determine if imaginary source
     register is set to "00000"
* 5. Removed imaginary source register's value determination
* 6. Removed imaginary source register's parameter set
*/

#include "../include/Instruction.h"

void lui_immd_assm(void) {
	if (strcmp(OP_CODE, "LUI") != 0) {
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

	// deleted param type check for second register
	// lui only has a destination register
	// Param 2 should be an immediate
	if (PARAM2.type != IMMEDIATE) {
		state = INVALID_IMMED;
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

	// deleted prior param 2 value check

	// immediate is limited to 16 bits, 0xF is 4 bits
	if (PARAM2.value > 0xFFFF) {
		state = INVALID_IMMED;
		return;
	}

	/*
		Putting the binary together
	*/
	
	// corrected setbit values 

	// Opcode set in first six bits from left
	setBits_str(31, "001111");

	// no source register but lui uses 5 bits after opcode as
	// imaginary source register
	setBits_str(25, "00000");

	// destination register set to 5 bits after imaginary source
	setBits_num(20, PARAM1.value, 5);

	// immediate set in last 16 bits
	setBits_num(15, PARAM2.value, 16); // Changed to set to immedate (15, value, 16)
	
	// Tell the system the encoding is done
	state = COMPLETE_ENCODE;
}

void lui_immd_bin(void) {
	// check if bits, 31-26, match load upper immediate opcode
	// check_bits(start_bit, bit_string) returns 0 if the bit_string matches

	// added check to make sure imaginary source register is always set to "00000"
	if (checkBits(31, "001111") != 0 || checkBits(25, "00000") != 0) {
		state = WRONG_COMMAND;
		//vif not matching return to caller
		return;
	}

	/*
		Finding values in the binary
	*/

	// Source register number determination deleted as source register
	// should always be imaginary "00000"
	
	// number for destination register determined from bits 20-16
	uint32_t Rt = getBits(20, 5);

	// offset value determined from bits 15-0
	uint32_t imm16 = getBits(15, 16);

	/*
		Setting Instuction values
	*/

	// set to load upper immediate operation
	setOp("LUI");

	// set destination register to determined register #
	// setParam(param_num, param_type, param_value)
	setParam(1, REGISTER, Rt);

	// set immediate to determined integer 
	setParam(2, IMMEDIATE, imm16); 

	// Tell the system the decoding is done
	state = COMPLETE_DECODE;
}