#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "exeptions.h"
#include "first_pass.h"
#include "utils.h"
#include "binary.h"



int data_image_line_create(DI_ptr *curr_line_ptr, DI_ptr *tail_ptr, char *currLine, long adress, int typeOfSentence, char *key_word, char* operands)
{	
	
	/*allocation*/
	DI_ptr curr = (dataImg*)malloc(sizeof(dataImg)), temp = NULL;
	if(!curr){print_errors(ALLOCATION_ERROR, adress);}
	
	switch(typeOfSentence)
	{
		case INSTRUCTION_LINE:
		{
			/*put the correct value*/
			instruction_binary_line(curr -> machineCode , key_word, operands);
			strcpy(curr -> sourceCode, currLine);
			curr -> adress = adress;
			curr -> next = NULL;
	
			/*update the line*/
			(*curr_line_ptr) = curr;
			return NEXT_ADRESS;
			break;
		}
		case GUIDANCE_LINE : 
		{
			/*declarations for guidances case*/
			int *operands_list = (int*)malloc(LINE_MAX_LENGTH * sizeof(int)); 			
			int i = 0, source_code_flag = on, lines_counter = 0; 
			long curr_adress = adress;			
	
			int num_of_operands =  to_ascii_list_operands(operands, operands_list); 

			/*go through the whole operands*/
			for(i = 0; i < num_of_operands; i++ )
			{	
				/*create the temporary line*/
				temp = (dataImg*)malloc(sizeof(dataImg));
				if(!temp){print_errors(ALLOCATION_ERROR, curr_adress);}
				
				/*add the machine code*/
				guidance_binary_lines(temp -> machineCode, key_word, operands_list[i]);

				/*add the source code*/
				if(source_code_flag == on)/*if it's the first table's line of the guidances line*/
				{
					strcpy(temp -> sourceCode, currLine); 
					source_code_flag = off;				
				}
				
				/*add the adress*/
				temp -> adress = curr_adress + lines_counter;

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

			return lines_counter;
			break;
		}
		
	}/*end of switch*/
		return FALSE;
}/*END instructionTabCreate()*/


symLine *add_to_symbol_table(char *label, long adress, int type_of_sentence)
{
	/*allocation*/
	symLine *curr_symbol_line = (symLine*)malloc(sizeof(symLine));

	/*adds the symbol and the value*/
	strcpy(curr_symbol_line -> symbol, label);
	curr_symbol_line -> value = adress;
	
	/*adds the arrtibutes*/
	switch(type_of_sentence)
	{
		case INSTRUCTION_LINE:
		{ 
			strcpy(curr_symbol_line -> attribute, "code ");
			break;
		}
		case GUIDANCE_LINE:
		{ 
			strcpy(curr_symbol_line -> attribute, "data ");
			break;
		}
		case EXTERN:
		{ 
			strcpy(curr_symbol_line -> attribute, "external ");
			break;
		}
	}/*end of switch*/

	curr_symbol_line -> next = NULL;
 	return curr_symbol_line;
}/*END add_to_symbol_table*/

void connect_adresses(dataImg *guidance_table_ptr, long last_ic)
{
	/*declarations*/
	int curr_adress = last_ic + NEXT_ADRESS;

	while(guidance_table_ptr != NULL)
	{
		guidance_table_ptr -> adress =  guidance_table_ptr -> adress + curr_adress;
		guidance_table_ptr = guidance_table_ptr -> next;	
	}

}/*EOF connect_adresses()*/

void free_symbol_table_nodes(symLine *head_symbol_line) 
{
	/*Go through the whole line*/
	while(head_symbol_line ->next != NULL)
	{
		free(head_symbol_line);
		head_symbol_line = head_symbol_line -> next; 
	}

}/*END free_symbol_table_nodes() */


void free_data_image_nodes(dataImg* head_data_image)
{
	/*Go through the whole line*/
	while(head_data_image -> next != NULL)
	{
		free(head_data_image);
		head_data_image = head_data_image -> next; 
	}

}/*END free_data_image_nodes()*/

/*-----------------delete from here down------------*/


void symbol_table_print(symLine *symbol_table_head)
{	
	printf("The Symbol Table:\n\n");
	while(symbol_table_head != NULL)
	{
		printf("symbol: %s\tvalue: %ld\tattribute: %s\n", symbol_table_head -> symbol, symbol_table_head -> value, symbol_table_head -> attribute);
		symbol_table_head = symbol_table_head -> next;
	}
	putchar('\n');

}


void data_image_print(dataImg *head)
{	
	printf("The Data Image:\n\n");
	while(head != NULL)
	{
		printf("adress: %ld\tsource code: %s\tmachine code: %s\n", head -> adress, head -> sourceCode,  head -> machineCode);
		head = head -> next;
	}
	putchar('\n');

}
