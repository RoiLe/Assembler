#ifndef BINARY_H
#define BINARY_H

#define ZERO 48
#define NINE 57
#define MIN_SIGNED_BYTE -128
#define MAX_SIGNED_BYTE 127
#define MIN_SIGNED_HALF_WORD -32768
#define MAX_SIGNED_HALF_WORD 32767
#define MIN_SIGNED_WORD -2147483647
#define MAX_SIGNED_WORD 2147483646


/*
	Creates a binary line from instructions lines in 
	every case of instruction line : R, I, J and specials.

	**The variables**
	curr_R_line -> current line of R structure [opcode, rs, rt, rd, funct, NONE]
	curr_I_line -> current line of I structure [opcode, rs, rt ,immed]
	curr_J_line -> current line of J structure [opcode, reg, address]
	curr_opcode -> the opcode code number
	curr_funct -> The funct code number
	type_of_key_word -> the type of the instruction line[R_LINE_LOGIC, R_LINE_COPY, I_LINE_LOGIC, I_LINE_BRANCH, I_LINE_SORE_LOAD, J_LINE_SPECIAL]
	 
	input output example: 	input -> add $3,$5,$9
				output -> 00000000011001010100100001000000
				
*/
void instruction_binary_line(char *machineCode, char *key_word , char *operands, int line_number);



/*
	Creates a binary line from instructions lines in 
	every case of instruction line : R, I, J and specials.

	**The variables**
	curr_db_asciz -> current line of db and asciz structure 
	curr_dh -> current line of dh structure 
	curr_dw -> current line of dw  structure 
	
	type_of_guidance -> the type of the guidances line[DB, DH, DW, ASCIZ, ENTRY, EXTERN]
	 
	input output example: 	input -> K: .dw 31,-12
				output ->00000000000000000000000000011111
					11111111111111111111111111110100
	
*/
void guidance_binary_lines(char *machine_code,char *key_word, long data, char *currLine, int line_number, int type_of_guidance);


/*
	Creates the structure of every line in integer
	return the current I line in I_struct
*/
I create_I_instruction(char *key_word, char *operands, int type_of_I, int curr_opcode, long curr_immed, int line_number); 


/*
	Creates the structure of every line in integer
	return the current J line in J_struct
*/
J create_J_instruction(char *key_word, char *operands, int curr_opcode, long adress, int line_number);


/*
	Converts from int to binary code [32 bits].
	return the binary code in string.

	input output example:	input -> 8
				output-> 00000000000000000000000000001000
*/
void int_to_binary(char* machine_code, int curr, int num_of_byte);








#endif
