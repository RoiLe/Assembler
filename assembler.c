
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "exeptions.h"
#include "globals.h"
#include "first_pass.h"
#include "utils.h"

/*
	The proccess of the two passes assembler.
	allocates memorys for the tables, than
	passes through the whole file and orgenaize
	the data. 
*/
int proccess_program(char*); 

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
	static long ic = IC;/*, dc = DC;*/
	FILE *assemblyCode = NULL;
	int exeptions = CORRECT, currentNumberLine = 0;
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
		int typeOfSentence;
		char *label = NULL;		
		char *currLine = (char *)calloc(LINE_MAX_LENGTH, sizeof(char));
		if(!currLine){print_errors(ALLOCATION_ERROR, currentNumberLine);}
		
		/*read the current line and define the type.*/
		fgets(currLine, LINE_MAX_LENGTH, assemblyCode);		
		typeOfSentence = classification_of_sentence(currLine);

		
		if(typeOfSentence == INSTRUCTION_LINE || typeOfSentence == GUIDANCE_LINE)		
		{
			currentNumberLine++;
			/*check the syntax of current line and print errors if there is.*/
			exeptions = check_line_syntax(currLine); 
			print_errors(exeptions, currentNumberLine);
			
			
			/*create the data image.*/
			if(typeOfSentence == INSTRUCTION_LINE)
			{		
				if(instruction_head == NULL)
				{
					data_image_line_create(&instruction_head, currLine, ic);
					instruction_head -> next = NULL;
					instruction_tail = instruction_head;
					/*printf("%d. instruction data image head line-->%s\n",currentNumberLine, instruction_head -> sourceCode);*//*test*/
				}else{					
					data_image_line_create(&p_instruction, currLine, ic);
					instruction_tail -> next = p_instruction;
					instruction_tail = instruction_tail -> next;
					/*printf("%d. instruction data image tail line-->%s\n",currentNumberLine, instruction_tail -> sourceCode);*//*test*/
				}
			
			}else{        
				if(guidance_head == NULL)
				{
					data_image_line_create(&guidance_head, currLine, ic);
					guidance_head -> next = NULL;
					guidance_tail = guidance_head;
					/*printf("%d. guidance data image head line-->%s\n",currentNumberLine, guidance_head -> sourceCode);*//*test*/
				}else{
					 data_image_line_create(&p_guidance, currLine, ic);
					 guidance_tail -> next = p_guidance;
					 guidance_tail = guidance_tail -> next;
					/*printf("%d. guidance data image tail line-->%s\n",currentNumberLine, guidance_tail -> sourceCode);*//*test*/
				}		
		 	
		 	}/*end if(typeOfSentence == INSTRUCTION_LINE*/
			
			
			
			/*create the symbol table*/	
			label = find_the_label(currLine);	
			if(label != NULL)
			{
				/*printf("round %d\n", i);test*/
				if(symbol_table_head == NULL)
				{
					symbol_table_head = add_to_symbol_table(label, ic);
					symbol_table_head -> next = NULL;
					symbol_table_tail = symbol_table_head;
				}else{
					p_symbol_table = add_to_symbol_table(label, ic);
					symbol_table_tail -> next = p_symbol_table;
					symbol_table_tail = symbol_table_tail -> next;
				}			
				
			}/*end if(label != NULL)*/
		ic++;
		} /*end if(typeOfSentence == INSTRUCTION_LINE || typeOfSentence == GUIDANCE_LINE)*/
		
		/*free some nodes*/
		free(currLine);    		
		free(label);
   	}
   	
   	fclose(assemblyCode);
	if(exeptions != CORRECT){return FAILED;}	

	/*connect the data image (instruction and then guidances)*/
	instruction_tail -> next = guidance_head;	
	data_image_print(instruction_head);
	symbol_table_print(symbol_table_head);
	
	/*
		the first pass done 
		if it's pass in success 
		we continue now to the 
		second pass..!!     
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
	
	
	
	/*
		free some nodes
	*/
	free_symbol_table_nodes(symbol_table_head);
	free_data_image_nodes(instruction_head);
	
	return SUCCESSED;
}/*END proccess_program()*/

