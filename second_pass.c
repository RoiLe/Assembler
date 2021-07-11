#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "exeptions.h"
#include "globals.h"
#include "utils.h"
#include "binary.h"
#include "second_pass.h"

long check_the_label_exist(symLine *symbol_table_head, char *label);
long get_current_adress(DI_ptr head_data_image, char *curr_line);
void change_the_immed(DI_ptr head_data_image, long new_immed, long current_adress, char *key_word, char *operands, int type_of_key_word, int curr_opcode);
void change_the_adress(DI_ptr head_data_image, char *key_word, char *operands, int curr_opcode, long new_adress, long current_adress);



void second_pass(DI_ptr instruction_head, symLine *symbol_table_head, char *curr_line)
{
	/*printf("%s ", curr_line);*/	



}/*END second_pass()*/



int add_entry_to_symbol_table(symLine *symbol_table_head, char *operand){

	while(symbol_table_head != NULL)
	{
		if(!strcmp(symbol_table_head -> symbol, operand))
		{
			if(!strcmp(symbol_table_head -> attribute, "external"))
				{
					printf("ERROR: there is a problem to define the same label as external and entry!\n");
					return FAILED;
				}
			strcat(symbol_table_head -> attribute, ", entry");
			return SUCCESSED;
		} 
		symbol_table_head = symbol_table_head -> next; 
	}/*end while loop*/	

	printf("ERROR: There is no matching label in the current file\n");

	return FAILED;
}/*END add_enrty_to_symbol_table()*/


void add_missed_values_to_data_image(DI_ptr instruction_head, symLine *symbol_table_head, char *key_word, char *operands, char *curr_line){
	int opcode = 0, funct = 0, reg_flag = off;
	int instruction_type = get_instruction_type(key_word, &opcode, &funct);
	char label[LABEL_MAX_LENGTH], curr_operands[LINE_MAX_LENGTH];
	long current_adress = 0, immed_value = 0, label_adress = 0;
		
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
				immed_value = current_adress - label_adress;

				/*change the machine code field.*/
				change_the_immed(instruction_head, immed_value, current_adress, key_word, curr_operands, instruction_type, opcode);
			}else{
				printf("ERROR: the label '%s' is not exist in the current code. \n", label);
			}

			break;
		}/*end I_LINE_BRANCH case*/
		case J_LINE_SPECIAL:
		{
			
			get_last_field(operands, label);
			
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
					change_the_adress(instruction_head, key_word, curr_operands, opcode, label_adress, current_adress);
				}
			}else if(!strcmp(key_word, "la") || !strcmp(key_word, "call")){
				/*check if the label its inside this file*/
				if(!(label_adress = check_the_label_exist(symbol_table_head, label)))
				{label_adress = off;}

				current_adress = get_current_adress(instruction_head, curr_line);

				/*change the adress field*/
				change_the_adress(instruction_head, key_word, curr_operands, opcode, label_adress, current_adress);		
			}

			break;
			
		}/*end J_LINE_SPECIAL case*/
	
	}/*end switch*/

}/*END add_missed_values_to_data_image()*/


long check_the_label_exist(symLine *symbol_table_head, char *label){
	
	while(symbol_table_head != NULL)
	{
		if(!strcmp(symbol_table_head -> symbol, label)){return symbol_table_head -> value;}
		symbol_table_head = symbol_table_head -> next; 
	}/*end while loop*/

	return 0;


}/*END check_the_label_exist()*/


long get_current_adress(DI_ptr head_data_image, char *curr_line){
	
	while(head_data_image != NULL)
	{
		if(!strcmp(head_data_image -> sourceCode, curr_line))
		{
			return head_data_image -> adress;
		}
		head_data_image = head_data_image -> next; 
	}/*end while loop*/

	return 0;

}/*END get_current_adress()*/


void change_the_immed(DI_ptr head_data_image, long new_immed, long current_adress, char *key_word, char *operands, int type_of_key_word, int curr_opcode){
	I curr_I_line;
	char *new_machine_code;	

	new_machine_code = (char *)calloc(MACHINE_CODE_LENGTH, sizeof(char));

	while(head_data_image != NULL)
	{
		if(head_data_image -> adress == current_adress)
		{
			curr_I_line = create_I_instruction(key_word, operands, type_of_key_word, curr_opcode, new_immed);
			int_to_binary(new_machine_code, curr_I_line.size, WORD);
			strcat(new_machine_code, "\0");
			strcpy(head_data_image -> machineCode, new_machine_code);
			
			free(new_machine_code);	
		}
		head_data_image = head_data_image -> next; 
	}/*end while loop*/



}/*END change_the_immed()*/

int check_reg_flag(DI_ptr head_data_image, char *curr_line){
	int reg_flag = off;	

	while(head_data_image != NULL)
	{
		if(!strcmp(head_data_image -> sourceCode, curr_line))
		{
			reg_flag = ((head_data_image -> machineCode[6]) == '1')?on:off;
			return reg_flag;
		}
		head_data_image = head_data_image -> next; 
	}/*end while loop*/

	return reg_flag;

}/*END check_reg_flag()*/ 


void change_the_adress(DI_ptr head_data_image, char *key_word, char *operands, int curr_opcode, long new_adress, long current_adress){
	J curr_J_line;
	char *new_machine_code;	

	new_machine_code = (char *)calloc(MACHINE_CODE_LENGTH, sizeof(char));

	while(head_data_image != NULL)
	{
		if(head_data_image -> adress == current_adress)
		{
			curr_J_line = create_J_instruction(key_word, operands, curr_opcode, new_adress);
			int_to_binary(new_machine_code, curr_J_line.size, WORD);
			strcat(new_machine_code, "\0");
			strcpy(head_data_image -> machineCode, new_machine_code);
			free(new_machine_code);	
		}
		head_data_image = head_data_image -> next; 
	}/*end while loop*/

}/*END change_the_adress()*/









