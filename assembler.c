#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "exeptions.h"
#include "globals.h"
#include "first_pass.h"
#include "utils.h"
#include "assembler.h"
#include "second_pass.h"
#include "to_files.h"


int main(int argc, char *argv[]){
	int success = 0;	
	int i;

	if (argc == 1)
	{
		printf("ERROR: there is no file. \n");
		exit(0); 
	}

	for(i = 1; i < argc; ++i) 
	{
		success = proccess_program(argv[i]);    /*the proccess done in succussefuly.*/
	}  	
	

return success;
}/*END main() */


 int proccess_program(char* file) 
{
	/*declarations*/
	static long ic = IC, dc = DC;
	FILE *assembly_code = NULL;
	int exeptions = CORRECT, currentNumberLine = 0, typeOfSentence;
	char label[LABEL_MAX_LENGTH], key_word[KEY_WORD_MAX_LENGTH], operands[LINE_MAX_LENGTH], the_name[20];
	DI_ptr instruction_head = NULL, instruction_tail = NULL, p_instruction;
	DI_ptr guidance_head = NULL, guidance_tail = NULL, p_guidance;

	symLine *symbol_table_head = NULL, *symbol_table_tail = NULL, *p_symbol_table;	
	

	/* checks the file name and open it if it's ok... */
	exeptions = name_check_file(file, the_name);	
	print_errors(exeptions, currentNumberLine);
	if(exeptions == CORRECT)
	{
		assembly_code = fopen(file, "r");/*open the file*/
		if(!assembly_code)
		{
			print_errors(DONT_EXIST_FILE_ERROR, currentNumberLine) ;
			return FAILED;
		}
	}
	else
	{
		print_errors(NAME_ERROR, currentNumberLine) ;
		return FAILED;
	}

	ic = IC;
	/*starting now the first pass... */
	while(!feof(assembly_code)) 
	{	
		/*declarations*/
		int guid_line_count;				
		char *currLine = (char *)calloc(LINE_MAX_LENGTH, sizeof(char));
		if(!currLine){print_errors(ALLOCATION_ERROR, currentNumberLine);}

		/*read the current line and define the type.*/
		fgets(currLine, LINE_MAX_LENGTH, assembly_code);		
		typeOfSentence = classification_of_sentence(currLine);
		/*printf("type: %d\t curennt line: %s\n",  typeOfSentence, currLine);*/

		/*printf("curr line: %s\n", currLine);*/
		if(typeOfSentence == INSTRUCTION_LINE || typeOfSentence == GUIDANCE_LINE)		
		{
			char temp_curr_line[LINE_MAX_LENGTH];
			strcpy(temp_curr_line, currLine);
			currentNumberLine++;

			/*get the data from the current line*/
			get_commands(temp_curr_line, label, key_word, operands);/*fill the label, the key word and the data of the line.*/		

			/*check the syntax of current line and print errors if there is.*/
			exeptions = check_line_syntax(currLine); 
			print_errors(exeptions, currentNumberLine);

			/*create the data image.*/
			
			if(typeOfSentence == INSTRUCTION_LINE )
			{		
				if(instruction_head == NULL)
				{
					ic += data_image_line_create(&instruction_head,NULL, currLine, ic, typeOfSentence, key_word, operands);
					instruction_head -> next = NULL;
					instruction_tail = instruction_head;
				}else{					
					ic += data_image_line_create(&p_instruction,NULL, currLine, ic, typeOfSentence, key_word, operands);
					instruction_tail -> next = p_instruction;
					instruction_tail = instruction_tail -> next;
				}
			}else{     
				if(guidance_head == NULL)
				{
					guid_line_count = data_image_line_create(&guidance_head, &guidance_tail, currLine, dc ,typeOfSentence, key_word, operands);
					
				}else{

					guid_line_count = data_image_line_create(&p_guidance, &guidance_tail, currLine, dc ,typeOfSentence, key_word, operands);
				}
				dc += guid_line_count;

		 	}/*end if(typeOfSentence == INSTRUCTION_LINE*/

			
			/*create the symbol table*/					
			if(label[0] != '\0' || !strcmp(key_word, ".extern"))
			{
				long curr_adress;

				strcat(label, "\0");
				if(typeOfSentence == INSTRUCTION_LINE)
					{curr_adress = ic - NEXT_ADRESS;}
				else
					{curr_adress = ic + dc - guid_line_count;}
	
				if(symbol_table_head == NULL)
				{
					if( !strcmp(key_word, ".extern"))
						{symbol_table_head = add_to_symbol_table(operands, off, EXTERN);}
					else
						{symbol_table_head = add_to_symbol_table(label, curr_adress, typeOfSentence);}

					symbol_table_head -> next = NULL;
					symbol_table_tail = symbol_table_head;
				}else{
					if( !strcmp(key_word, ".extern"))
						{p_symbol_table = add_to_symbol_table(operands, off, EXTERN);}
					else
						{p_symbol_table = add_to_symbol_table(label, curr_adress, typeOfSentence);}

					symbol_table_tail -> next = p_symbol_table;
					symbol_table_tail = symbol_table_tail -> next;
				}			

			}/*end if(label != NULL)*/
		
		} /*end if(typeOfSentence == INSTRUCTION_LINE || typeOfSentence == GUIDANCE_LINE)*/	

		/*free current line*/
		free(currLine); 
		
		/*clear the vars*/  
		label[0] = '\0'; 
		operands[0] = '\0';
		key_word [0] = '\0';
		
   	}/*END while(!feof(assembly_code)) loop */
   	
	if(exeptions != CORRECT){return FAILED;}	

	/*printf("instruction\n\n\t\t");
	data_image_print(instruction_head);
	printf("guidances\n\n\t\t");
	data_image_print(guidance_head);*/
	
	/*order the adresses of the guidances lines after the instructions lines*/
	connect_adresses(guidance_head, instruction_tail -> adress);

	/*connect the data images (instruction and then guidances)*/
	instruction_tail -> next = guidance_head;

	
	/*print the first pass table	
	printf("\tfirst pass table\n\n");
	data_image_print(instruction_head);
	symbol_table_print(symbol_table_head);*/

	/*complete the values of data attributes in symbol table*/
	insert_values_to_data_attribute(symbol_table_head, instruction_head);

	/*
		the first pass done!  if it's pass in success, 
		we continue now to the second pass..!!     
	*/
	rewind(assembly_code);
		
	while(!feof(assembly_code)) 
	{
		char *currLine = (char *)calloc(LINE_MAX_LENGTH, sizeof(char));
		if(!currLine){print_errors(ALLOCATION_ERROR, currentNumberLine);}		

		fgets(currLine, LINE_MAX_LENGTH, assembly_code);		
		typeOfSentence = classification_of_sentence(currLine);

		if(typeOfSentence == INSTRUCTION_LINE || typeOfSentence == GUIDANCE_LINE)/*if its not empty or note lines*/		
		{
			char temp_curr_line[LINE_MAX_LENGTH];
			strcpy(temp_curr_line, currLine);
	
			get_commands(temp_curr_line, label, key_word, operands);/*fill the label, the key word and the data of the line.*/		

			if(typeOfSentence == GUIDANCE_LINE && get_guidance_type(key_word) == ENTRY)
			{
				add_entry_to_symbol_table(symbol_table_head, operands);
			}else{
				add_missed_values_to_data_image(instruction_head, symbol_table_head, key_word, operands, currLine);				
			}
		}
		free(currLine);
	}/*end of while loop*/
	
	/*close the assembley code file*/
	fclose(assembly_code);

	printf("\n\n\tsecond pass tables\n\n");
	data_image_print(instruction_head);
	symbol_table_print(symbol_table_head);
	
	/*
		the second pass done 
		if it's pass in success we continue now to the  
		output..!!
	*/
	
	outputs(symbol_table_head, instruction_head, ic, dc, the_name);

	/*free some nodes*/
	free_symbol_table_nodes(symbol_table_head);
	free_data_image_nodes(instruction_head);

	/*reset the counters*/
	ic = IC; 
	dc = DC;

	return SUCCESSED;
}/*END proccess_program()*/

