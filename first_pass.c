#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "exeptions.h"
#include "first_pass.h"
#include "binary.h"


symLine *add_to_symbol_table(char *label, long adress)
{
	/*allocation*/
	symLine *curr_symbol_line = (symLine*)malloc(sizeof(symLine));
	
	strcpy(curr_symbol_line -> symbol, label);
	curr_symbol_line -> value = adress;
	curr_symbol_line -> attribute = "null";
	curr_symbol_line -> next = NULL;
	
 	return curr_symbol_line;
}/*END add_to_symbol_table*/


void data_image_line_create(DI_ptr *line_ptr, char *currLine, long adress)
{
	/*allocation*/
	DI_ptr curr = (dataImg*)malloc(sizeof(dataImg));
	if(!curr){print_errors(ALLOCATION_ERROR, 1);}

	strcpy(curr -> sourceCode, currLine);
	curr -> adress = adress;
	curr -> next = NULL;
	strcpy(curr -> machineCode, binaryLine(1));
	
	(*line_ptr) = curr;

}/*END instructionTabCreate()*/


void free_symbol_table_nodes(symLine *head_symbol_line)
{
	while(head_symbol_line ->next != NULL)
	{
		free(head_symbol_line);
		head_symbol_line = head_symbol_line -> next; 
	}

}


void free_data_image_nodes(dataImg* head_data_image)
{
	while(head_data_image -> next != NULL)
	{
		free(head_data_image);
		head_data_image = head_data_image -> next; 
	}

}

/*-----------------delete from here down------------*/


void symbol_table_print(symLine *symbol_table_head)
{	
	printf("The Symbol Table:\n\n");
	while(symbol_table_head != NULL)
	{
		printf("value: %ld\t attribute: %s\t symbol: %s\n", symbol_table_head -> value, symbol_table_head -> attribute, symbol_table_head -> symbol);
		symbol_table_head = symbol_table_head -> next;
	}
	putchar('\n');

}


void data_image_print(dataImg *head)
{	
	printf("The Data Image:\n\n");
	while(head != NULL)
	{
		/*printf("adress: %ld\n", head -> adress);*/
		printf("adress: %ld\tsource code: %s\tmachine code: %s\n", head -> adress, head -> sourceCode,  head -> machineCode);
		head = head -> next;
	}
	putchar('\n');

}
