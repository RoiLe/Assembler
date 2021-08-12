#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "exeptions.h"
#include "globals.h"
#include "utils.h"
#include "binary.h"
#include "second_pass.h"



/*
	get the adress from the data image.
	return the adress
*/
long get_current_adress(DI_ptr head_data_image, char *curr_line);

/*
	update the immed field in I instrution lines.

	curr_I_line -> the updated I line.
	new_machine_code -> new binary code.
*/
void change_the_immed(DI_ptr head_data_image, long new_immed, long current_adress, char *key_word, char *operands, int type_of_key_word, int curr_opcode, int number_line);

/*
	update the adress fields in j instruction lines. 

	curr_J_line -> the updated j line.
	new_machine_code -> new binary code.
	
*/
void change_the_adress(DI_ptr head_data_image, char *key_word, char *operands, int curr_opcode, long new_adress, long current_adress, int number_line);

/*
	get the corrct adress to insert to the symbol table to the "data" attributes.
	return the adress. 
*/
long get_adress_from_data_image(char* label, DI_ptr  data_image_head);

/*
	get the label from the source code.
*/
void get_label(char *source_code, char *label);




int add_entry_to_symbol_table(symLine *symbol_table_head, char *operand, int number_line){

	int none = 0;
	/*go through the symbol table*/
	while(symbol_table_head != NULL)
	{
		if(strcmp(symbol_table_head -> symbol, operand) == 0)/*if the label exist*/
		{
			if(!strcmp(symbol_table_head -> attribute, "external"))/*it's already external*/
				{
					/*it's not allowed to define both, external and entry */
					print_errors(EXTERNAL_ENTRY_TOGETHER_ERROR, number_line, &none, operand);
					return FAILED;
				}
			/*add the entry attribute*/
			strcat(symbol_table_head -> attribute, ", entry");
			return SUCCESSED;
		} 
		
		symbol_table_head = symbol_table_head -> next; 
	}/*end while loop*/

	/*the label is not exist in the data image. */	
	print_errors(LABEL_DONT_EXIST, number_line, &none, operand);

	return FAILED;
}/*END add_enrty_to_symbol_table()*/


void add_missed_values_to_data_image(DI_ptr instruction_head, symLine *symbol_table_head, char *key_word, char *operands, char *curr_line, int number_line){
	/*declarations*/
	int opcode = 0, funct = 0, reg_flag = off, none = 0;
	int instruction_type = get_instruction_type(key_word, &opcode, &funct);
	char label[LABEL_MAX_LENGTH], curr_operands[LINE_MAX_LENGTH];
	long current_adress = 0, immed_value = 0, label_adress = 0;
	
	/*copy of the current operands*/	
	strcpy(curr_operands, operands);
	strcat(curr_operands, "\0");	

	switch(instruction_type)
	{
		case I_LINE_BRANCH:
		{
			/*get the label*/
			get_last_field(operands, label);

			/*check if the label its inside this file*/
			if((label_adress = check_the_label_exist(symbol_table_head, label)))
			{
				current_adress = get_current_adress(instruction_head, curr_line);
				immed_value = label_adress - current_adress;/*distance between them*/

				/*change the machine code field.*/
				change_the_immed(instruction_head, immed_value, current_adress, key_word, curr_operands, instruction_type, opcode, number_line);
			}else{print_errors(SYNTAX_ERROR, number_line, &none, curr_line);}

			break;
		}/*end I_LINE_BRANCH case*/
		case J_LINE_SPECIAL:
		{	
			if(operands[0] != '\0')	/*just stop with no label*/	
			{get_last_field(operands, label);}
		
			if(!strcmp(key_word, "jmp"))
			{
				/*the reg flag*/
				reg_flag = check_reg_flag(instruction_head, curr_line);
				if(reg_flag == off)
				{
					/*check if the label its inside this file*/
					if(!(label_adress = check_the_label_exist(symbol_table_head, label)))
					{label_adress = off;}
					
					current_adress = get_current_adress(instruction_head, curr_line);

					/*change the adress field*/
					change_the_adress(instruction_head, key_word, curr_operands, opcode, label_adress, current_adress, number_line);
				}
			}else if(!strcmp(key_word, "la") || !strcmp(key_word, "call")){
				/*check if the label its inside this file*/
				if(!(label_adress = check_the_label_exist(symbol_table_head, label)))
					{label_adress = off;}

				current_adress = get_current_adress(instruction_head, curr_line);

				/*change the adress field*/
				change_the_adress(instruction_head, key_word, curr_operands, opcode, label_adress, current_adress, number_line);		
			}
			break;			
		}/*end J_LINE_SPECIAL case*/
	
	}/*end switch*/

}/*END add_missed_values_to_data_image()*/



long get_current_adress(DI_ptr head_data_image, char *curr_line){
	/*go through the whole data image */
	while(head_data_image != NULL)
	{
		/*if it's the same line*/
		if(!strcmp(head_data_image -> sourceCode, curr_line))
		{
			/*get the adress*/
			return head_data_image -> adress;
		}
		head_data_image = head_data_image -> next; 
	}/*end while loop*/

	return 0;

}/*END get_current_adress()*/


void change_the_immed(DI_ptr head_data_image, long new_immed, long current_adress, char *key_word, char *operands, int type_of_key_word, int curr_opcode, int number_line){
	/*declarations*/	
	I curr_I_line;
	char *new_machine_code;	

	/*allocation*/
	new_machine_code = (char *)calloc(MACHINE_CODE_LENGTH, sizeof(char));

	/*go through the whole data image */
	while(head_data_image != NULL)
	{
		if(head_data_image -> adress == current_adress)
		{
			/*recreate the current line*/
			curr_I_line = create_I_instruction(key_word, operands, type_of_key_word, curr_opcode, new_immed, number_line);
			int_to_binary(new_machine_code, curr_I_line.size, WORD);
			strcat(new_machine_code, "\0");
			strcpy(head_data_image -> machineCode, new_machine_code);
			
			free(new_machine_code);	
		}
		head_data_image = head_data_image -> next; 
	}/*end while loop*/



}/*END change_the_immed()*/


int check_reg_flag(DI_ptr head_data_image, char *curr_line){
	/*declarations*/
	int reg_flag = off, reg_flag_position = 6;	

	/*go through the whole data image */
	while(head_data_image != NULL)
	{
		/*check the position of the register flag*/
		if(!strcmp(head_data_image -> sourceCode, curr_line))
		{
			reg_flag = ((head_data_image -> machineCode[reg_flag_position]) == '1')?on:off;
			return reg_flag;
		}
		head_data_image = head_data_image -> next; 
	}/*end while loop*/
	return reg_flag;
}/*END check_reg_flag()*/ 


void change_the_adress(DI_ptr head_data_image, char *key_word, char *operands, int curr_opcode, long new_adress, long current_adress, int number_line){
	/*declarations*/
	J curr_J_line;
	char *new_machine_code;	

	/*allocation*/
	new_machine_code = (char *)calloc(MACHINE_CODE_LENGTH, sizeof(char));

	/*go through the whole data image */
	while(head_data_image != NULL)
	{
		/*if we get to the correct place in the table*/
		if(head_data_image -> adress == current_adress)
		{
			/*recreate the current line*/
			curr_J_line = create_J_instruction(key_word, operands, curr_opcode, new_adress, number_line);
			int_to_binary(new_machine_code, curr_J_line.size, WORD);
			strcat(new_machine_code, "\0");
			strcpy(head_data_image -> machineCode, new_machine_code);
			
			free(new_machine_code);	
		}
		head_data_image = head_data_image -> next; 
	}/*end while loop*/

}/*END change_the_adress()*/

void insert_values_to_data_attribute(symLine *symbol_table_head, DI_ptr data_image_head){
	/*declarations*/
	long adress = 0;
	char label[LABEL_MAX_LENGTH] = "\0";

	/*go through the whole symbol table */
	while(symbol_table_head -> next != NULL)
	{
		/*put the value in the correct place */
		if(!strcmp(symbol_table_head -> attribute, "data"))
		{
			strcpy(label, symbol_table_head -> symbol);
			
			adress = get_adress_from_data_image(label, data_image_head);
			symbol_table_head -> value = adress;
		}
		symbol_table_head = symbol_table_head -> next;
	}/*end while loop*/
}/*END adds_values_to_data_attribute()*/


long get_adress_from_data_image(char* label, DI_ptr data_image_head){
	/*declarations*/
	char temp_label[LABEL_MAX_LENGTH] = "\0";

	/*go through the whole data image to find the adress */
	while(data_image_head -> next != NULL)
	{
		get_label(data_image_head -> sourceCode, temp_label);
		if(!strcmp(temp_label, label))
		{			
			return data_image_head -> adress;
		}
		data_image_head = data_image_head -> next;
	}/*end while loop*/
	
	return FALSE;
}/*END  get_adress_from_data_image()*/


void get_label(char *source_code, char *label){
	/*declarations*/
	int i = 0;
	char temp_label[LABEL_MAX_LENGTH] = "\0";

	/*go through the whole source code to find the label */
	while(source_code[i] != '\0')
	{
		temp_label[i] = source_code[i];
		i++;
		if(source_code[i] == ':'){strcpy(label, temp_label);}

	}/*end while loop*/ 
}/*END get_label()*/
