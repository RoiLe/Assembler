#ifndef SECONDPASS_H
#define SECONDPASS_H


/*


*/
/*void second_pass(DI_ptr instruction_head, symLine *symbol_table_head, char *curr_line);*/


/*
	adds the entry attributes to the symbol table
	Goes through the entire symbol table and adds the "entry" attrbute. 

	if the symbol is already external, it's forbid to adds an entry attribute also.

	return success or failed. [add or not]
*/
int add_entry_to_symbol_table(symLine *symbol_table_head, char *operand, int number_line);


/*
	adds the values that we didn't know in the first pass.
	adds to I_LINE_BRANCH type and J_LINE_SPECIAL type the distance to the label.
	(just if the label define in the current file)
	

	**variables**
	opcode -> Have this variable for the get_instruction_type() func.
	funct -> Have this variable for the get_instruction_type() func.
	reg_flag -> Tell us if there is a register in the current line.
	instruction_type -> The type of the instruction line.
	label -> The current label if there is. 
	curr_operands -> The current operands. 
	current_adress -> The cuurent adress. 
	immed_value -> The value that we add. 
	label_adress -> The adress we need to check to the distance. 

*/
void add_missed_values_to_data_image(DI_ptr instruction_head,symLine *symbol_table_head,char *key_word, char *operands, char *curr_line, int number_line);

/*
	check if there is a register in the current line. 
	
	**variable**
	reg_flag -> Displays the status of the register [on/off]
	return the flag. 	

*/
int check_reg_flag(DI_ptr head_data_image, char *curr_line);


/*
	puts the values in the right place in the image.
 
	**variables**
	adress -> The adress that we put into the value. 
	label -> A variable that helps us keep the current label.

*/
void insert_values_to_data_attribute(symLine *symbol_table_head, DI_ptr data_image_head);

#endif
