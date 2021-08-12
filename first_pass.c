#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "exeptions.h"
#include "first_pass.h"
#include "utils.h"
#include "binary.h"


int data_image_line_create(DI_ptr *curr_line_ptr, DI_ptr *tail_ptr, char *currLine, long adress, int type_of_sentence, char *key_word, char* operands, int line_number)
{	
	int none = 0;

	/*allocation*/
	DI_ptr curr = (dataImg*)malloc(sizeof(dataImg)), temp = NULL;
	if(!curr){print_errors(ALLOCATION_ERROR, off, &none, NULL);}
	
	switch(type_of_sentence)
	{
		case INSTRUCTION_LINE:
		{
			/*put the correct value*/
			instruction_binary_line(curr -> machineCode , key_word, operands, line_number);
			strcpy(curr -> sourceCode, currLine);
			curr -> adress = adress;
			curr -> next = NULL;
	
			/*update the line*/
			(*curr_line_ptr) = curr;
			return NEXT_ADRESS;
			break;
		}/*end of instruction case*/
		case GUIDANCE_LINE : 
		{
			/*declarations for guidances case*/
			int *operands_list = (int*)malloc(LINE_MAX_LENGTH * sizeof(int)); 			
			int i = 0, source_code_flag = on, lines_counter = 0, type_of_line = 0, half_byte = 2;			 
			long curr_adress = adress;	
		
	 		int num_of_operands =  to_ascii_list_operands(operands, operands_list, line_number);

			/*allocation problem*/
			if(!operands_list){print_errors(ALLOCATION_ERROR, off, &none, NULL);}

			/*gets and checks the guidance line*/
			type_of_line = get_guidance_type(key_word);
			if(type_of_line == FALSE){print_errors(KEY_WORD_INCORRECT, line_number, off, key_word);}

			/*go through the whole operands*/
			for(i = 0; i < num_of_operands; i++ )
			{	
				/*creates the temporary line*/
				temp = (dataImg*)malloc(sizeof(dataImg));
				if(!temp){print_errors(ALLOCATION_ERROR, off, &none, NULL);}
				
				/*adds every operand to the machine code*/
				guidance_binary_lines(temp -> machineCode, key_word, operands_list[i], currLine, line_number, type_of_line);

				/*adds the source code*/
				if(source_code_flag == on)/*if it's the first table's line of the guidances line*/
				{
					strcpy(temp -> sourceCode, currLine); 
					source_code_flag = off; 				
				}

				
				/*add the adress*/
				switch(type_of_line)
				{
				case DB:
				case ASCIZ:
					temp -> adress = curr_adress + lines_counter;
					break;
				case DH:
					temp -> adress = curr_adress + lines_counter;
					curr_adress += half_byte;
					break;
				case DW:
					temp -> adress = curr_adress + lines_counter;
					curr_adress += NEXT_ADRESS - 1;
					break;
				
				}/*end switch*/
				

				/*update the current line*/
				if((*curr_line_ptr) == NULL)
				{
					(*curr_line_ptr) = temp;
					(*curr_line_ptr) -> next = NULL;
					(*tail_ptr) = (*curr_line_ptr);
				}else{
					(*tail_ptr) -> next = temp;
					temp -> next = NULL;
					(*tail_ptr) = temp;
				}
				lines_counter++;
			}/*end of for loop*/
			
			switch(type_of_line)
			{
				case DH:
					lines_counter++;
					break;
				case DW:
					lines_counter += NEXT_ADRESS + half_byte;
					break;					
			}/*end add type_of_line switch*/
			return lines_counter;
			break;
		}/*end of guidances case*/
		
	}/*end of switch*/
		return FALSE;
}/*END instructionTabCreate()*/


symLine *add_to_symbol_table(char *label, long adress, int type_of_sentence)
{
	/*allocation*/
	symLine *curr_symbol_line = (symLine*)malloc(sizeof(symLine));

	/*adds the symbol and the value*/
	strcpy(curr_symbol_line -> symbol, label);
	
	
	/*adds the arrtibutes*/
	switch(type_of_sentence)
	{
		case INSTRUCTION_LINE:
		{ 
			strcpy(curr_symbol_line -> attribute, "code");
			curr_symbol_line -> value = adress;
			break;
		}
		case GUIDANCE_LINE:
		{ 
			strcpy(curr_symbol_line -> attribute, "data");
			break;
		}
		case EXTERN:
		{ 
			strcpy(curr_symbol_line -> attribute, "external");
			break;
		}
	}/*end of switch*/

	curr_symbol_line -> next = NULL;
 	return curr_symbol_line;
}/*END add_to_symbol_table*/


void connect_adresses(dataImg *guidance_table_ptr, long last_ic)
{
	/*declarations*/
	long curr_adress = last_ic + NEXT_ADRESS;

	while(guidance_table_ptr != NULL) 
	{
		guidance_table_ptr -> adress =  guidance_table_ptr -> adress + curr_adress;
		guidance_table_ptr = guidance_table_ptr -> next;	
	}

}/*EOF connect_adresses()*/



void free_symbol_table_nodes(symLine *head_symbol_line) 
{
	/*Go through the whole line*/
	while(head_symbol_line -> next == NULL)
	{
		free(head_symbol_line);
		head_symbol_line = head_symbol_line -> next; 
	}

}/*END free_symbol_table_nodes() */


void free_data_image_nodes(dataImg* head_data_image)
{
	/*Go through the whole line*/
	while(head_data_image -> next == NULL)
	{
		free(head_data_image);
		head_data_image = head_data_image -> next; 
	}

}/*END free_data_image_nodes()*/

 

/*-----------------------not in use--------------------------*/
/*---------unless you want to see the prints-----------------*/

void symbol_table_print(symLine *symbol_table_head)
{	
	printf("The Symbol Table:\n\n");
	while(symbol_table_head != NULL)
	{
		printf("symbol: %s\tvalue: %ld\tattribute: %s\n", symbol_table_head -> symbol, symbol_table_head -> value, symbol_table_head -> attribute);
		symbol_table_head = symbol_table_head -> next;
	}
	putchar('\n');

}/*END symbol_table_print()*/


void data_image_print(dataImg *head)
{	
	printf("The Data Image:\n\n");
	while(head != NULL)
	{
		printf("adress: %ld\tsource code: %s\tmachine code: %s\n", head -> adress, head -> sourceCode,  head -> machineCode);
		head = head -> next;
	}
	putchar('\n');

}/*END data_image_print()*/
