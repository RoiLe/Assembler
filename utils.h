#ifndef UTILS_H
#define UTILS_H

#define WHITE_SPACE_SKIP while(operands[i] == ' ' || operands[i] == '\n' || operands[i] == '\t'){i++;}

enum TypeOfSentence{
	EMPTY_LINE = 1,
	NOTE_LINE,
	INSTRUCTION_LINE,
	GUIDANCE_LINE 
};


/*
	Checks what the type of the sentense, go through the whole line
	and return the type. 
	[EMPTY_LINE, NOTE_LINE, INSTRUCTION_LINE, GUIDANCE_LINE ]

	**variables**
	empty_flag -> if its an empty line, the flag will turn on.
	guidance_flag -> if it's guidance lines, the flag will turn on. 
*/
int classification_of_sentence(char* curr_line);

/*
	Takes a string and turns it over.
	Use it for turn over the binary code. 
	
	**variables**
	start -> first character of the string. 
	end -> last character of the string. 

	example: input -  00000000000000000000000000111110
		output - 01111100000000000000000000000000
*/
char *revese_string(char* str);

/*
	Get the label, key word and operands of the current line. 
	go through the whole line. 

	**variables**
	places -> the palaces of the command in the line. 
	p -> pointer for the strtok func.
   	array -> array of the command that we divide.

*/
void get_commands(char *curr_line, char* label, char* key_word, char* operands);


/*
	This function checks what type of instruction line,
	and also brings the the current opcode funct. 
	
	**variables**
	
	size of key word arrays:
	num_of_R_logic
	num_of_R_copy
	num_of_I_logic
	num_of_I_branch
	num_of_I_store_load
	num_of_J_special

	arrays of diffrent type of key word:
	R_logic_key_word
	R_copy_key_word
	I_logic_key_words
	I_branch_key_word
	I_store_load_key_word
	J_special_key_word
	
	funct's array: 
	R_funct_array

	opcode's arrays:
 	R_opcode_array
	I_opcode_array
	J_opcode_array
*/
int get_instruction_type(char *key_word, int *curr_opcode, int *curr_funct);

/*
	This function checks what type of guidances line,
	and also brings the the current opcode funct. 

	**variables**

	num_of_types -> the number of options.
	guidance_type -> the all possible ke word in array.
*/
int get_guidance_type(char *key_word);

/*
	This func takes arrays of string values, 
	and return array of numbers that more easy to change to binary code.

	**variables**

	quote_flag -> check if there is quote.
	operands_counter -> number of operands.
	comma_flag -> mark the comma.
	minus_flag -> negative number.
	temp_num -> temporary number. 

*/
int to_ascii_list_operands(char* operands, int *the_list); 

#endif