
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "exeptions.h"
#include "globals.h"
#include "first_pass.h"
#include "utils.h"
#include "assembler.h"


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
	FILE *assemblyCode = NULL;
	int exeptions = CORRECT, currentNumberLine = 0;
	char label[LABEL_MAX_LENGTH], key_word[KEY_WORD_MAX_LENGTH], operands[LINE_MAX_LENGTH];
	DI_ptr instruction_head = NULL, instruction_tail = NULL, p_instruction;
	DI_ptr guidance_head = NULL, guidance_tail = NULL, p_guidance;

	symLine *symbol_table_head = NULL, *symbol_table_tail = NULL, *p_symbol_table;

	/* checks the file name and open it if it's ok... */
	exeptions = name_check_file(file);	
	print_errors(exeptions, currentNumberLine);
	if(exeptions == CORRECT)
	{
		assemblyCode = fopen(file, "r");/*open the file*/
		if(!assemblyCode)
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


	/*starting now the first pass... */
	while(!feof(assemblyCode)) 
	{	
		/*declarations*/
		int typeOfSentence, guid_line_count;				
		char *currLine = (char *)calloc(LINE_MAX_LENGTH, sizeof(char));

		if(!currLine){print_errors(ALLOCATION_ERROR, currentNumberLine);}
		
		/*read the current line and define the type.*/
		fgets(currLine, LINE_MAX_LENGTH, assemblyCode);		
		typeOfSentence = classification_of_sentence(currLine);

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
				if(typeOfSentence == INSTRUCTION_LINE)
					{curr_adress = ic - NEXT_ADRESS;}
				else
					{curr_adress = ic + dc - guid_line_count;}
	
				if(symbol_table_head == NULL)
				{
					if( !strcmp(key_word, ".extern"))
						{symbol_table_head = add_to_symbol_table(operands, off, EXTERN);}
					else
					{					
						symbol_table_head = add_to_symbol_table(label, curr_adress, typeOfSentence);
					}
					symbol_table_head -> next = NULL;
					symbol_table_tail = symbol_table_head;
				}else{
					if( !strcmp(key_word, ".extern"))
						{p_symbol_table = add_to_symbol_table(operands, off, EXTERN);}
					else
					{
						p_symbol_table = add_to_symbol_table(label, curr_adress, typeOfSentence);
					}
					symbol_table_tail -> next = p_symbol_table;
					symbol_table_tail = symbol_table_tail -> next;
				}			


			}/*end if(label != NULL)*/
		
		} /*end if(typeOfSentence == INSTRUCTION_LINE || typeOfSentence == GUIDANCE_LINE)*/	

		/*free nodes*/
		free(currLine); 
		
		/*clear the vars*/  
		label[0] = '\0'; 
		operands[0] = '\0';
		key_word [0] = '\0';
		
   	}
   	
   	fclose(assemblyCode);
	if(exeptions != CORRECT){return FAILED;}	

	/*order the adresses of the guidances lines after the instructions lines*/
	connect_adresses(guidance_head, instruction_tail -> adress);

	/*connect the data image (instruction and then guidances)*/
	instruction_tail -> next = guidance_head;	
	data_image_print(instruction_head);
	symbol_table_print(symbol_table_head);
	
	/*
		the first pass done!  if it's pass in success, 
		we continue now to the second pass..!!     
	*/	
	
	/*rewind(assemblerCode);
	while(the table is didnt done)
	{
		secondPass(symboleTable, dataImage);
	}*/
	
	
	/*
		the second pass done 
		if it's pass in success 
		we continue now to the  
		output..!!
	*/

	/*outPut(dataImage);*/
	
	
	
	/*free some nodes*/
	free_symbol_table_nodes(symbol_table_head);
	free_data_image_nodes(instruction_head);

	return SUCCESSED;
}/*END proccess_program()*/

